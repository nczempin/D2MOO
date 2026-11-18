// ============================================================================
// Diablo II 1.12a D2Client.dll - Reconstructed Translation Unit: UI_dialog.cpp
// Source: ..\Source\D2Client\UI\dialog.cpp (0x6FB12960..0x6FB14190)
// ImageBase: 0x6FAB0000
// ============================================================================

#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "D2ClientReconstructed.h"

struct D2UnitStrc;
struct D2CellFileStrc;
struct Unicode {
    uint16_t ch;
    static int __fastcall strlen(const Unicode* pStr);
    static Unicode* __fastcall strncpy(Unicode* pDst, const Unicode* pSrc, int nCount);
};

struct D2DialogMsgStrc {
    HWND hWnd;              // +0x00
    uint32_t uMsg;          // +0x04
    union {
        uint32_t wParam;    // +0x08
        struct {
            uint16_t wKey;      // +0x08
            int16_t nWheelDelta;// +0x0A
        };
    };
    union {
        uint32_t lParam;    // +0x0C
        struct {
            uint16_t wMouseX;   // +0x0C
            uint16_t wMouseY;   // +0x0E
        };
    };
    uint32_t dwCommand;     // +0x10
    uint32_t dwKeyFlags;    // +0x14
    int32_t bHandled;       // +0x18
    int32_t lResult;        // +0x1C
};

struct D2DialogWidgetBaseStrc {
    struct VTable {
        void (__thiscall* pfnDestroy)(D2DialogWidgetBaseStrc*, int);
        void (__thiscall* pfnDraw)(D2DialogWidgetBaseStrc*);
        int (__thiscall* pfnOnMouseWheel)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLButtonDown)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLButtonUp)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnMouseMove)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnKeyDown)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnUpArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnDownArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnLeftArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
        int (__thiscall* pfnOnRightArrow)(D2DialogWidgetBaseStrc*, void* pMsg);
    }* pVtbl;
};

struct D2DialogWidgetNodeStrc {
    D2DialogWidgetBaseStrc* pWidget;      // +0x00
    D2DialogWidgetNodeStrc* pNext;        // +0x04
};

struct D2DialogLineStrc {
    uint16_t wszText[120];                // +0x00
    int32_t nLineHeight;                  // +0xF0
    int32_t nTextOffsetX;                 // +0xF4
    int32_t nTextWidth;                   // +0xF8
    int32_t nUnkF4;                       // +0xFC
    int32_t nFont;                        // +0x100
    int32_t nColor;                       // +0x104
    int (__cdecl* pfnAction)(void);       // +0x108
    int32_t bSelectable;                  // +0x10C
};

struct D2DialogStrc {
    uint32_t dwCreationTick;              // +0x00
    uint32_t dwUnk04;                     // +0x04
    D2CellFileStrc* pBackgroundCel;       // +0x08
    int32_t bHasBackgroundCel;            // +0x0C
    int32_t bMousePressedInDialog;        // +0x10
    int32_t bUseNonSelectableHeightAnchor;// +0x14
    int32_t bBlockUnhandledKeys;          // +0x18
    int32_t bCloseOnRightClick;           // +0x1C
    int32_t bIgnoreMenuHotkeys;           // +0x20
    int32_t nAnchorX;                     // +0x24
    int32_t nAnchorY;                     // +0x28
    int32_t nPosX;                        // +0x2C
    int32_t nPosY;                        // +0x30
    int32_t nWidth;                       // +0x34
    int32_t nHeight;                      // +0x38
    int32_t nUnk3C;                       // +0x3C
    int32_t nUnk40;                       // +0x40
    int32_t nSelectedIndex;               // +0x44
    int32_t nHoveredIndex;                // +0x48
    int32_t nSelectableLineCount;         // +0x4C
    int32_t nLineCount;                   // +0x50
    uint32_t dwFrameCount;                // +0x54
    int32_t bAutoCalcBounds;              // +0x58
    int32_t nSelectionCursorMode;         // +0x5C
    int (__cdecl* pfnCancel)(void);       // +0x60
    int (__cdecl* pfnOutsideClick)(void); // +0x64
    D2DialogLineStrc aLines[10];          // +0x68..+0xB07
    D2DialogWidgetNodeStrc* pWidgetList;  // +0xB08
    D2DialogStrc* pPrevDialog;            // +0xB0C
};

struct D2SMsgRegEntryStrc {
    uint32_t nDispatchType;
    uint32_t dwMsgOrKey;
    void (__stdcall* pfnHandler)(void* pMsg);
};

extern "C" {
    void* __fastcall FOG_AllocClientMemory(int nSize, const char* szFile, int nLine, int nZero);
    void __fastcall FOG_FreeClientMemory(void* pMem, const char* szFile, int nLine, int nZero);
    const char* __cdecl Fog_10265(int nLine);
    void __cdecl FOG_DisplayHalt(const char* szMsg, const char* szFile, int nLine);
    int __stdcall SMsgBreakHandlerChain(void* pMsg);
    HWND __stdcall WINDOW_GetWindow();
    void __stdcall TEXTURE_CelDraw(D2GfxDataStrc* pData, int nX, int nY, uint32_t dwGamma, int nDrawMode, uint8_t* pPalette);
    void __stdcall D2GFX_DrawBox(int nXStart, int nYStart, int nXEnd, int nYEnd, uint8_t nColor, int nDrawMode);
    int __fastcall D2Win_10127_SetFont(int nFont);
    int __fastcall D2Win_10121_GetTextWidth(const Unicode* wszText);
    void __fastcall D2Win_10117_DrawText(const Unicode* wszText, int nX, int nY, int nColor, int bCentered);
    int __stdcall D2CMP_CelFileFreeHardware(D2CellFileStrc* pCellFile);
    void* __stdcall D2CMP_CelGetHandle(D2GfxDataStrc* pGfxData);
    int __stdcall D2CMP_CelGetWidth(void* pCelHandle);
    int __stdcall D2CMP_CelGetHeight(void* pCelHandle);
}

static inline void FOG_DisplayHalt(const char* szFmt, const char* szLoc)
{
    FOG_DisplayHalt(szFmt ? szFmt : "Halt", szLoc ? szLoc : __FILE__, 0);
}

extern void __fastcall WALL2_UnregisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern void __fastcall WALL2_RegisterMsgHandlerTable(uint32_t nCount, HWND hWnd, const D2SMsgRegEntryStrc* pEntries);
extern D2CellFileStrc* __fastcall ARCHIVE_LoadCellFileDefault(const char* szFileName, int32_t nFileType);
extern void __fastcall UI_DrawBoxPiecesFrame(RECT* pRect);
extern void __fastcall UI_StartPanelCloseClickCooldown(void);
extern int32_t __fastcall GAME_HasMouseWheelB(void);
extern int32_t __fastcall CMDTBL_HasKeyBound(int32_t nCommandId, int32_t bIsPrimary);
extern uint16_t __fastcall CMDTBL_GetBoundKey(int32_t nCommandId, int32_t bIsPrimary);
extern void __stdcall INV_ClearHoveredItemState();
extern void __stdcall SOUND_PlaySoundEffect(uint32_t nSoundId, D2UnitStrc* pUnit, int32_t nX, int32_t nY, BOOL bGlobal);
extern int32_t __fastcall UI_HitTestNewSkillBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY);
extern int32_t __fastcall UI_HitTestNewStatBtn640(int32_t nMouseX, int32_t nSplitMode, int32_t nMouseY);
extern int32_t __fastcall D2Client_HitTestMiniPanel(int32_t nMouseX);
extern void __stdcall AUTOMAP_NoOp(int32_t nUnused);
extern void __stdcall AUTOMAP_PanRight(int32_t nUnused);
extern void __stdcall AUTOMAP_PanLeft(int32_t nUnused);
extern void __stdcall AUTOMAP_PanUp(int32_t nUnused);
extern void __stdcall AUTOMAP_PanDown(int32_t nUnused);

extern int32_t g_bLeftMouseHeld;
extern int32_t g_bLeftMouseRepeating;
extern int32_t g_bRightMouseHeld;
extern int32_t g_bRightMouseRepeating;
extern int32_t gnTooltipUnitType;
extern int32_t gnTooltipUnitGUID;

void __stdcall DIALOG_OnMouseButtonUp(void* pMsg);
void __stdcall DIALOG_OnEscapeKey(void* pMsg);
void __stdcall DIALOG_OnMenuCancelHotkey(void* pMsg);
void __stdcall DIALOG_OnReturnKey(void* pMsg);
void __stdcall DIALOG_OnCharKey(void* pMsg);
void __stdcall DIALOG_OnDownArrow(void* pMsg);
void __stdcall DIALOG_OnUpArrow(void* pMsg);
void __stdcall DIALOG_OnLeftArrow(void* pMsg);
void __stdcall DIALOG_OnRightArrow(void* pMsg);
void __stdcall DIALOG_OnMouseWheel(void* pMsg);
void __stdcall DIALOG_OnMouseButtonDown(void* pMsg);
void __stdcall DIALOG_OnMouseMove(void* pMsg);

// 1.12a: 0x6FBA7DA8 (Automap arrow SMsg table)
static const D2SMsgRegEntryStrc s_aAutomapArrowHandlers[5] = {
    { 3, VK_UP,    reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanUp)    },
    { 3, VK_DOWN,  reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanDown)  },
    { 3, VK_RIGHT, reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanRight) },
    { 3, VK_LEFT,  reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_PanLeft)  },
    { 3, VK_END,   reinterpret_cast<void (__stdcall*)(void*)>(AUTOMAP_NoOp)     },
};

// 1.12a: 0x6FBA8D90 (Dialog SMsg handler registration table, 14 entries)
static D2SMsgRegEntryStrc s_aDialogMsgHandlers[14] = {
    { 3, VK_ESCAPE,      DIALOG_OnMenuCancelHotkey },
    { 3, VK_ESCAPE,      DIALOG_OnMenuCancelHotkey },
    { 0, WM_LBUTTONUP,   DIALOG_OnMouseButtonUp    },
    { 0, WM_LBUTTONDOWN, DIALOG_OnMouseButtonDown  },
    { 0, WM_RBUTTONUP,   DIALOG_OnMouseButtonUp    },
    { 0, WM_RBUTTONDOWN, DIALOG_OnMouseButtonDown  },
    { 3, VK_UP,          DIALOG_OnUpArrow          },
    { 3, VK_DOWN,        DIALOG_OnDownArrow        },
    { 3, VK_RIGHT,       DIALOG_OnRightArrow       },
    { 3, VK_LEFT,        DIALOG_OnLeftArrow        },
    { 3, VK_RETURN,      DIALOG_OnReturnKey        },
    { 3, VK_ESCAPE,      DIALOG_OnEscapeKey        },
    { 0, WM_CHAR,        DIALOG_OnCharKey          },
    { 0, WM_MOUSEMOVE,   DIALOG_OnMouseMove        },
};

// 1.12a: 0x6FBA8E38 (Dialog mouse-wheel SMsg registration table)
static const D2SMsgRegEntryStrc s_aDialogWheelHandlers[1] = {
    { 0, WM_MOUSEWHEEL,  DIALOG_OnMouseWheel       },
};

// ============================================================================
// Dialog Module Globals (1.12a D2Client.dll .data / .bss)
// ============================================================================
D2DialogStrc*   g_pActiveDialog            = nullptr; // 0x6FBCA784
int32_t         g_bDialogOpen              = 0;       // 0x6FBCA788
int32_t         g_nActiveDialogCount       = 0;       // 0x6FBCA78C
D2CellFileStrc* g_pPentagramCel            = nullptr; // 0x6FBCA790
uint32_t        g_dwPentagramFrame         = 0;       // 0x6FBCA794
int32_t         g_nDialogModalRefCount     = 0;       // 0x6FBCA798
RECT            g_rcDialogBounds           = { 0 };   // 0x6FBCA79C
int32_t         g_bDialogBoundsValid       = 0;       // 0x6FBCA7AC
static int32_t  s_bDialogCursorInvalidated = 0;       // 0x6FBCA7B0
static int32_t  s_nDialogBorderPadX        = 16;      // 0x6FBA8D88
static int32_t  s_nDialogBorderPadY        = 16;      // 0x6FBA8D8C
static const char* const g_apszDialogBgNames[] = { "dialogbackground" }; // 0x6FBA8D84

// PART 1: UI\dialog.cpp (0x6FB12960..0x6FB14190, 39 functions)
// ============================================================================

// 1.12a: 0x6FB12960 (__cdecl)
int32_t __cdecl DIALOG_GetActiveDialogCount()
{
    return g_nActiveDialogCount;
}

// 1.12a: 0x6FB12970 (register: eax=pDialog, ecx=pOutRect)
void __fastcall DIALOG_GetBoundsRect(RECT* pOutRect, D2DialogStrc* pDialog)
{
    if (pDialog && pOutRect) {
        pOutRect->top = pDialog->nPosY;
        pOutRect->left = pDialog->nPosX;
        pOutRect->bottom = pDialog->nPosY + pDialog->nHeight;
        pOutRect->right = pDialog->nPosX + pDialog->nWidth;
    }
}

// 1.12a: 0x6FB129A0 (register: ecx=pDialog, eax=nLineIdx)
D2DialogLineStrc* __fastcall DIALOG_GetLine(D2DialogStrc* pDialog, int nLineIdx)
{
    if (pDialog && nLineIdx >= 0 && nLineIdx < pDialog->nLineCount) {
        return &pDialog->aLines[nLineIdx];
    }
    return nullptr;
}

// 1.12a: 0x6FB129C0 (register: eax=pDialog)
int32_t __fastcall DIALOG_GetSelectedIndex(D2DialogStrc* pDialog)
{
    return pDialog ? pDialog->nSelectedIndex : -1;
}

// 1.12a: 0x6FB129D0 (register: edi=pDialog, eax=nLineIdx)
void __fastcall DIALOG_RecalcLineLayout(D2DialogStrc* pDialog, int nLineIdx)
{
    int nOldFont = D2Win_10127_SetFont(1);
    D2DialogLineStrc* pLine = &pDialog->aLines[nLineIdx];
    D2Win_10127_SetFont(pLine->nFont);
    int nTextWidth = D2Win_10121_GetTextWidth(reinterpret_cast<const Unicode*>(pLine->wszText));
    pLine->nTextWidth = nTextWidth;
    if (nTextWidth >= pDialog->nWidth) {
        pLine->nTextOffsetX = 0;
    } else {
        pLine->nTextOffsetX = ((pDialog->nWidth - nTextWidth + 1) / 2) + 1;
    }
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB12A40 (register: eax=pDialog, stack: nMode)
void __stdcall DIALOG_SetSelectionCursorMode(D2DialogStrc* pDialog, int32_t nMode)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", Fog_10265(0x529));
        exit(-1);
    }
    pDialog->nSelectionCursorMode = nMode;
}

// 1.12a: 0x6FB12A70 (register: ecx=pDialog, esi=nSelectedIndex)
void __fastcall DIALOG_SetSelectedIndex(D2DialogStrc* pDialog, int32_t nSelectedIndex)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", Fog_10265(0x4F2));
        exit(-1);
    }
    if (nSelectedIndex >= pDialog->nLineCount) {
        FOG_DisplayHalt("%s", Fog_10265(0x4F8));
        exit(-1);
    }
    if (nSelectedIndex == -1) {
        pDialog->nSelectedIndex = -1;
        return;
    }
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        if (i == nSelectedIndex) {
            if (!pDialog->aLines[i].bSelectable) {
                FOG_DisplayHalt("%s", Fog_10265(0x51A));
                exit(-1);
            }
            pDialog->nSelectedIndex = nSelectedIndex;
            return;
        }
    }
    FOG_DisplayHalt("%s", Fog_10265(0x51E));
    exit(-1);
}

// 1.12a: 0x6FB12B00 (register: esi=pDialog, stack: pWidget)
void __stdcall DIALOG_AttachWidget(D2DialogStrc* pDialog, D2DialogWidgetBaseStrc* pWidget)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", Fog_10265(0x358));
        exit(-1);
    }
    D2DialogWidgetNodeStrc* pOldHead = pDialog->pWidgetList;
    D2DialogWidgetNodeStrc* pNode = static_cast<D2DialogWidgetNodeStrc*>(
        FOG_AllocClientMemory(sizeof(D2DialogWidgetNodeStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x35C, 0));
    pDialog->pWidgetList = pNode;
    pNode->pNext = pOldHead;
    pNode->pWidget = pWidget;
}

// 1.12a: 0x6FB12B60 (register: edx=pDialog)
void __fastcall DIALOG_ResetStruct(D2DialogStrc* pDialog)
{
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->nSelectedIndex = -1;
    pDialog->nSelectionCursorMode = 2;
}

// 1.12a: 0x6FB12BA0 (__cdecl)
void __cdecl DIALOG_ClearOpenFlag()
{
    g_bDialogOpen = 0;
}

// 1.12a: 0x6FB12BB0 (register: esi=pDialog, eax=pMsg)
int32_t __fastcall DIALOG_HitTestSelectableLine(D2DialogStrc* pDialog, void* pMsg)
{
    if (!pDialog) {
        return -1;
    }
    int nMouseX = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseX;
    int nMouseY = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseY;
    int nAccumY = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        D2DialogLineStrc* pLine = &pDialog->aLines[i];
        if (pLine->bSelectable == 1) {
            if (nMouseX > pDialog->nPosX + 15 && nMouseX < pDialog->nPosX + pDialog->nWidth - 15) {
                int nLineBottomY = pDialog->nPosY + nAccumY + pLine->nLineHeight;
                if (nMouseY > nLineBottomY - 11 && nMouseY < nLineBottomY + 4) {
                    return i;
                }
            }
        }
        nAccumY += pLine->nLineHeight;
    }
    return -1;
}

// 1.12a: 0x6FB12C40 (register: edx=pDialog)
void __fastcall DIALOG_SelectFirstSelectableLine(D2DialogStrc* pDialog)
{
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        if (pDialog->aLines[i].bSelectable) {
            pDialog->nSelectedIndex = i;
            return;
        }
    }
}

// 1.12a: 0x6FB12C70 (__cdecl)
D2DialogStrc* __cdecl DIALOG_GetActiveDialog()
{
    return g_pActiveDialog;
}

// 1.12a: 0x6FB12C80 (register: eax=pDialog)
void __fastcall DIALOG_AutoCalcBounds(D2DialogStrc* pDialog)
{
    if (!pDialog->bAutoCalcBounds) {
        FOG_DisplayHalt("%s", Fog_10265(0x5AA));
        exit(-1);
    }
    int nOldFont = D2Win_10127_SetFont(1);
    int nMaxWidth = 0;
    int nTotalHeight = 0;
    int nNonSelectableCount = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        D2Win_10127_SetFont(pDialog->aLines[i].nFont);
        int nLineWidth = D2Win_10121_GetTextWidth(reinterpret_cast<const Unicode*>(pDialog->aLines[i].wszText));
        if (nLineWidth > nMaxWidth) {
            nMaxWidth = nLineWidth;
        }
        if (!pDialog->aLines[i].bSelectable) {
            nNonSelectableCount++;
        }
        nTotalHeight += pDialog->aLines[i].nLineHeight;
    }
    nMaxWidth += 68;
    if (nMaxWidth < 144) {
        nMaxWidth = 144;
    }
    nTotalHeight += 18;
    D2Win_10127_SetFont(nOldFont);

    pDialog->nPosX = pDialog->nAnchorX - (nMaxWidth / 2);
    int nOffsetY = pDialog->bUseNonSelectableHeightAnchor ? (nNonSelectableCount * 21) : (nTotalHeight / 3);
    pDialog->nPosY = pDialog->nAnchorY - nOffsetY;

    int nScreenW = gnScreenWidth;
    int nScreenH = gnScreenHeight;
    if (nMaxWidth > nScreenW - 0x50) {
        FOG_DisplayHalt("%s", Fog_10265(0x5DD));
        exit(-1);
    }
    if (nTotalHeight > nScreenH - 0x62) {
        FOG_DisplayHalt("%s", Fog_10265(0x5E2));
        exit(-1);
    }
    if (pDialog->nPosX + nMaxWidth > nScreenW - 10) {
        pDialog->nPosX = nScreenW - nMaxWidth;
    }
    if (pDialog->nPosY + nTotalHeight > nScreenH - 0x3A) {
        pDialog->nPosY = nScreenH - nTotalHeight - 0x30;
    }
    if (pDialog->nPosX <= 10) pDialog->nPosX = 10;
    if (pDialog->nPosY <= 10) pDialog->nPosY = 10;

    pDialog->nWidth = nMaxWidth;
    pDialog->nHeight = nTotalHeight;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        DIALOG_RecalcLineLayout(pDialog, i);
    }
}

// 1.12a: 0x6FB12DF0 (register: ecx=pDialog, eax=nColor, stack: wszText, nLineHeight, nUnkF4, nFont, pfnAction, bSelectable)
void __stdcall DIALOG_AppendLine(D2DialogStrc* pDialog, int nColor, const Unicode* wszText, int nLineHeight, int nUnkF4, int nFont, int (__cdecl* pfnAction)(void), int bSelectable)
{
    int nIdx = pDialog->nLineCount;
    if (nIdx >= 10) {
        FOG_DisplayHalt("%s", Fog_10265(0x55D));
        exit(-1);
    }
    pDialog->nLineCount = nIdx + 1;
    D2DialogLineStrc* pLine = &pDialog->aLines[nIdx];
    pLine->nLineHeight = nLineHeight;
    pLine->nTextOffsetX = 0;
    pLine->nUnkF4 = nUnkF4;
    pLine->nFont = nFont;
    if (nColor == 3 && pDialog->nSelectionCursorMode == 1) {
        FOG_DisplayHalt("%s", Fog_10265(0x569));
        exit(-1);
    }
    pLine->nColor = nColor;
    pLine->pfnAction = pfnAction;
    pLine->bSelectable = bSelectable;

    int nLen = Unicode::strlen(wszText) + 1;
    if (nLen > 119) nLen = 119;
    Unicode::strncpy(reinterpret_cast<Unicode*>(pLine->wszText), wszText, nLen);
    pLine->wszText[nLen] = 0;

    if (bSelectable) {
        pDialog->nSelectableLineCount++;
    }
    if (pDialog->bAutoCalcBounds) {
        DIALOG_AutoCalcBounds(pDialog);
    } else {
        DIALOG_RecalcLineLayout(pDialog, nIdx);
    }

    g_rcDialogBounds.left = pDialog->nPosX;
    g_rcDialogBounds.top = pDialog->nPosY;
    g_rcDialogBounds.right = pDialog->nPosX + pDialog->nWidth;
    g_rcDialogBounds.bottom = pDialog->nPosY + pDialog->nHeight;
    g_bDialogBoundsValid = 1;

    if (pDialog->nSelectedIndex == -1 && bSelectable) {
        DIALOG_SetSelectedIndex(pDialog, nIdx);
    }
}

// 1.12a: 0x6FB12F40 (register: esi=pDialog)
void __fastcall DIALOG_Render(D2DialogStrc* pDialog)
{
    if (!pDialog) {
        FOG_DisplayHalt("%s", Fog_10265(0x499));
        exit(-1);
    }
    pDialog->dwFrameCount++;
    RECT rcBox;
    rcBox.left = pDialog->nPosX;
    rcBox.top = pDialog->nPosY;
    rcBox.right = pDialog->nPosX + pDialog->nWidth - 1;
    rcBox.bottom = pDialog->nPosY + pDialog->nHeight - 1;

    D2GfxDataStrc gfxData = {};
    if (pDialog->bHasBackgroundCel) {
        if (pDialog->pBackgroundCel) {
            gfxData.pCellFile = pDialog->pBackgroundCel;
            gfxData.nFrame = 0;
            TEXTURE_CelDraw(&gfxData, rcBox.left, rcBox.bottom + 1, 0xFFFFFFFF, 5, nullptr);
        }
    } else {
        D2GFX_DrawBox(rcBox.left, rcBox.top, rcBox.right + 1, rcBox.bottom + 1, 0, 1);
        UI_DrawBoxPiecesFrame(&rcBox);
    }

    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget) {
            pNode->pWidget->pVtbl->pfnDraw(pNode->pWidget);
        }
    }

    int nOldFont = D2Win_10127_SetFont(1);
    int nAccumY = 0;
    for (int i = 0; i < pDialog->nLineCount; ++i) {
        D2DialogLineStrc* pLine = &pDialog->aLines[i];
        int nColor = pLine->nColor;
        D2Win_10127_SetFont(pLine->nFont);
        int nDrawX = pDialog->nPosX + pLine->nTextOffsetX;
        int nDrawY = pDialog->nPosY + nAccumY + pLine->nLineHeight;
        if (pDialog->nSelectedIndex == i) {
            if (pDialog->nSelectionCursorMode == 1) {
                nColor = 3;
            } else if (pDialog->nSelectionCursorMode == 2) {
                if (!g_pPentagramCel) {
                    g_pPentagramCel = ARCHIVE_LoadCellFileDefault("CURSOR\\focus16", 0);
                }
                if (g_pPentagramCel) {
                    extern int32_t __stdcall D2CMP_CelFileGetCelsPerDirection(D2CellFileStrc* pCellFile);
                    int32_t nSpinFrames = D2CMP_CelFileGetCelsPerDirection(g_pPentagramCel);
                    int32_t nAnimIdx = (nSpinFrames > 0) ? static_cast<int32_t>((GetTickCount() / 60u) % static_cast<uint32_t>(nSpinFrames)) : 0;
                    gfxData.pCellFile = g_pPentagramCel;
                    gfxData.nFrame = nAnimIdx;
                    int32_t nIconW = 16;
                    if (void* pCelH = D2CMP_CelGetHandle(&gfxData)) {
                        int32_t nQueryW = D2CMP_CelGetWidth(pCelH);
                        if (nQueryW > 0 && nQueryW <= 32) {
                            nIconW = nQueryW;
                        }
                    }
                    TEXTURE_CelDraw(&gfxData, nDrawX - nIconW - 5, nDrawY + 2, 0xFFFFFFFF, 5, nullptr);
                    gfxData.nFrame = (nSpinFrames > 1) ? (nSpinFrames - 1 - nAnimIdx) : nAnimIdx;
                    TEXTURE_CelDraw(&gfxData, nDrawX + pLine->nTextWidth + 5, nDrawY + 2, 0xFFFFFFFF, 5, nullptr);
                    g_dwPentagramFrame++;
                }
            }
        }
        D2Win_10117_DrawText(reinterpret_cast<const Unicode*>(pLine->wszText), nDrawX, nDrawY, nColor, 0);
        nAccumY += pLine->nLineHeight;
    }
    D2Win_10127_SetFont(nOldFont);
}

// 1.12a: 0x6FB13120 (register: eax=pMsg, stack: bInvokeCancel)
void __stdcall DIALOG_ConsumeMsgAndOptionalCancel(void* pMsg, int bInvokeCancel)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    if (bInvokeCancel) {
        D2DialogStrc* pDialog = g_pActiveDialog;
        if (!pDialog) {
            return;
        }
        if (pDialog->dwFrameCount >= 1 && pDialog->pfnCancel) {
            extern DWORD g_dwPanelCloseCooldownTick;
            pDialog->pfnCancel();
            UI_StartPanelCloseClickCooldown();
            g_dwPanelCloseCooldownTick = GetTickCount() + 500u;
        }
    }
}

// 1.12a: 0x6FB13180 (__stdcall)
void __stdcall DIALOG_OnMouseButtonUp(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    int nMouseX = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseX;
    int nMouseY = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseY;
    if (gaPanelStates[D2PANEL_MINIPANEL]) {
        int nHalfW = gnScreenWidth / 2;
        int nScreenH = gnScreenHeight;
        if (nMouseX > nHalfW - 0x4A && nMouseX < nHalfW + 0x47 && nMouseY < nScreenH - 0x32 && nMouseY > nScreenH - 0x45) {
            if (!pDialog) return;
            if (pDialog->bMousePressedInDialog != 1) pDialog->bMousePressedInDialog = 0;
            return;
        }
    }
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    if (!pDialog) {
        return;
    }
    if (pDialog->bMousePressedInDialog != 1) {
        for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
            if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLButtonUp(pNode->pWidget, pMsg) == 1) {
                return;
            }
        }
        return;
    }
    if (pDialog->dwCreationTick + 80 > GetTickCount()) {
        return;
    }
    int nHitLine = DIALOG_HitTestSelectableLine(pDialog, pMsg);
    if (pDialog->nHoveredIndex == nHitLine && nHitLine >= 0 && nHitLine < pDialog->nLineCount) {
        auto pfnAction = pDialog->aLines[nHitLine].pfnAction;
        if (pfnAction && pfnAction()) {
            return;
        }
    }
    pDialog->bMousePressedInDialog = 0;
}

// 1.12a: 0x6FB13290 (__cdecl)
void __cdecl DIALOG_CloseActive()
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog || g_nActiveDialogCount <= 0) {
        g_nActiveDialogCount = 0;
        g_pActiveDialog = nullptr;
        g_bDialogOpen = 0;
        g_bDialogBoundsValid = 0;
        return;
    }
    --g_nActiveDialogCount;
    if (g_nActiveDialogCount == 0) {
        if (gaPanelStates[D2PANEL_AUTOMAP]) {
            WALL2_RegisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
        }
        WALL2_UnregisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
        if (GAME_HasMouseWheelB()) {
            WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
        }
        s_bDialogCursorInvalidated = 1;
        g_bDialogBoundsValid = 0;
        g_bDialogOpen = 0;
    }

    D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList;
    while (pNode) {
        D2DialogWidgetNodeStrc* pNext = pNode->pNext;
        FOG_FreeClientMemory(pNode, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x430, 0);
        pNode = pNext;
    }
    if (pDialog->bHasBackgroundCel && pDialog->pBackgroundCel) {
        D2CMP_CelFileFreeHardware(pDialog->pBackgroundCel);
        FOG_FreeClientMemory(pDialog->pBackgroundCel, "..\\Source\\D2Client\\CORE\\ARCHIVE.CPP", 0x7A, 0);
    }
    D2DialogStrc* pPrev = pDialog->pPrevDialog;
    FOG_FreeClientMemory(pDialog, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x43B, 0);
    g_pActiveDialog = pPrev;
}

// 1.12a: 0x6FB133B0 (__cdecl)
void __cdecl DIALOG_BindMenuCancelHotkeys()
{
    if (CMDTBL_HasKeyBound(0x26, 1)) {
        uint16_t wKey = CMDTBL_GetBoundKey(0x26, 1);
        if (wKey != 0xFFFF && wKey <= 0xDF) {
            s_aDialogMsgHandlers[0].dwMsgOrKey = wKey;
        }
    }
    if (CMDTBL_HasKeyBound(0x26, 0)) {
        uint16_t wKey = CMDTBL_GetBoundKey(0x26, 0);
        if (wKey != 0xFFFF && wKey <= 0xDF) {
            s_aDialogMsgHandlers[1].dwMsgOrKey = wKey;
        }
    }
}

// 1.12a: 0x6FB13460 (__cdecl)
void __cdecl DIALOG_DestroyAll()
{
    if (g_bDialogOpen) {
        WALL2_UnregisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
        if (GAME_HasMouseWheelB()) {
            WALL2_UnregisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
        }
        g_nDialogModalRefCount--;
    }
    D2DialogStrc* pCur = g_pActiveDialog;
    while (pCur) {
        D2DialogStrc* pPrev = pCur->pPrevDialog;
        FOG_FreeClientMemory(pCur, "..\\Source\\D2Client\\UI\\dialog.cpp", 0x336, 0);
        pCur = pPrev;
    }
    g_pActiveDialog = nullptr;
}

// 1.12a: 0x6FB134E0 (register: eax=pDialog, edi=nKey)
int32_t __fastcall DIALOG_IsIgnoredMenuHotkey(D2DialogStrc* pDialog, int nKey)
{
    if (!pDialog || pDialog->bIgnoreMenuHotkeys || gaPanelStates[D2PANEL_CHAT] == 0) {
        return 0;
    }
    if (CMDTBL_HasKeyBound(5, 1) && CMDTBL_GetBoundKey(5, 1) == nKey) {
        return 1;
    }
    if (CMDTBL_HasKeyBound(5, 0) && CMDTBL_GetBoundKey(5, 0) == nKey) {
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB13540 (__stdcall)
void __stdcall DIALOG_OnEscapeKey(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (pDialog && pDialog->dwCreationTick + 80 > GetTickCount()) {
        static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
        static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    if (pDialog->dwFrameCount >= 1 && pDialog->pfnCancel) {
        extern DWORD g_dwPanelCloseCooldownTick;
        pDialog->pfnCancel();
        UI_StartPanelCloseClickCooldown();
        g_dwPanelCloseCooldownTick = GetTickCount() + 500u;
    }
}

// 1.12a: 0x6FB135D0 (__stdcall)
void __stdcall DIALOG_OnMenuCancelHotkey(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    if (pDialog->bIgnoreMenuHotkeys || gaPanelStates[D2PANEL_CHAT] == 0) {
        DIALOG_ConsumeMsgAndOptionalCancel(pMsg, pDialog->bCloseOnRightClick == 0);
    }
}

// 1.12a: 0x6FB13600 (__stdcall)
// 1.12a: 0x6FB13630 (continuation block of DIALOG_CreateFramed)
D2DialogStrc* __stdcall DIALOG_CreateFramed(
    int nAnchorX, int nAnchorY,
    int (__cdecl* pfnCancel)(void),
    int (__cdecl* pfnOutsideClick)(void),
    int bBlockUnhandledKeys,
    int bUseNonSelectableHeightAnchor,
    int bAutoCalcBounds,
    int nWidth, int nHeight,
    int bCloseOnRightClick,
    int bIgnoreMenuHotkeys)
{
    if (g_nActiveDialogCount == 0 && gaPanelStates[D2PANEL_AUTOMAP]) {
        WALL2_UnregisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
    }
    INV_ClearHoveredItemState();
    s_nDialogBorderPadX = 16;
    s_nDialogBorderPadY = 16;
    s_bDialogCursorInvalidated = 0;
    g_bLeftMouseHeld = 0;
    g_bLeftMouseRepeating = 0;
    g_bRightMouseHeld = 0;
    g_bRightMouseRepeating = 0;
    gnTooltipUnitGUID = 0;
    gnTooltipUnitType = 0;
    if (nAnchorX == 0) {
        return nullptr;
    }
    const bool bFirstDialog = (g_nActiveDialogCount == 0);
    g_nActiveDialogCount++;
    D2DialogStrc* pDialog = static_cast<D2DialogStrc*>(
        FOG_AllocClientMemory(sizeof(D2DialogStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x3ED, 0));
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->pPrevDialog = g_pActiveDialog;
    g_pActiveDialog = pDialog;

    pDialog->nAnchorX = nAnchorX;
    pDialog->nPosX = nAnchorX;
    pDialog->nAnchorY = nAnchorY;
    pDialog->nPosY = nAnchorY;
    pDialog->pfnCancel = pfnCancel;
    pDialog->pfnOutsideClick = pfnOutsideClick;
    pDialog->bBlockUnhandledKeys = bBlockUnhandledKeys;
    pDialog->bUseNonSelectableHeightAnchor = bUseNonSelectableHeightAnchor;
    pDialog->bAutoCalcBounds = bAutoCalcBounds;
    pDialog->nWidth = nWidth;
    pDialog->nHeight = nHeight;
    pDialog->bCloseOnRightClick = bCloseOnRightClick;
    pDialog->bIgnoreMenuHotkeys = bIgnoreMenuHotkeys;
    pDialog->nSelectedIndex = -1;
    pDialog->nHoveredIndex = -1;
    pDialog->nSelectionCursorMode = 2;
    pDialog->dwCreationTick = GetTickCount();

    g_bDialogOpen = 1;
    if (bFirstDialog) {
        DIALOG_BindMenuCancelHotkeys();
        WALL2_RegisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
        if (GAME_HasMouseWheelB()) {
            WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
        }
        g_nDialogModalRefCount++;
    }
    return pDialog;
}

// 1.12a: 0x6FB13800 (__stdcall)
// 1.12a: 0x6FB13850 (continuation block of DIALOG_CreateWithBackgroundCel)
D2DialogStrc* __stdcall DIALOG_CreateWithBackgroundCel(
    int nPosX, int nPosY,
    int (__cdecl* pfnCancel)(void),
    int (__cdecl* pfnOutsideClick)(void),
    int nBgNameIdx,
    int bBlockUnhandledKeys)
{
    if (gaPanelStates[D2PANEL_AUTOMAP]) {
        WALL2_UnregisterMsgHandlerTable(5, WINDOW_GetWindow(), s_aAutomapArrowHandlers);
    }
    s_nDialogBorderPadX = 16;
    s_nDialogBorderPadY = 16;
    s_bDialogCursorInvalidated = 0;
    INV_ClearHoveredItemState();
    g_bLeftMouseHeld = 0;
    g_bLeftMouseRepeating = 0;
    g_bRightMouseHeld = 0;
    g_bRightMouseRepeating = 0;
    gnTooltipUnitGUID = 0;
    gnTooltipUnitType = 0;
    if (nPosX == 0 || !pfnCancel) {
        return nullptr;
    }
    g_nActiveDialogCount++;
    D2DialogStrc* pDialog = static_cast<D2DialogStrc*>(
        FOG_AllocClientMemory(sizeof(D2DialogStrc), "..\\Source\\D2Client\\UI\\dialog.cpp", 0x399, 0));
    memset(pDialog, 0, sizeof(D2DialogStrc));
    pDialog->pPrevDialog = g_pActiveDialog;
    g_pActiveDialog = pDialog;

    pDialog->nSelectedIndex = -1;
    pDialog->nHoveredIndex = -1;
    pDialog->nSelectionCursorMode = 2;
    pDialog->pfnCancel = pfnCancel;
    pDialog->pfnOutsideClick = pfnOutsideClick;
    pDialog->dwCreationTick = GetTickCount();
    pDialog->bBlockUnhandledKeys = bBlockUnhandledKeys;
    pDialog->bHasBackgroundCel = 1;

    char szPath[260];
    wsprintfA(szPath, "%s\\ui\\menu\\%s", "DATA\\GLOBAL", g_apszDialogBgNames[nBgNameIdx]);
    pDialog->pBackgroundCel = ARCHIVE_LoadCellFileDefault(szPath, 0);

    D2GfxDataStrc gfxData = {};
    gfxData.pCellFile = pDialog->pBackgroundCel;
    void* pCelHandle = D2CMP_CelGetHandle(&gfxData);
    if (!pCelHandle) {
        FOG_DisplayHalt("%s", Fog_10265(0x3B6));
        exit(-1);
    }
    pDialog->nWidth = D2CMP_CelGetWidth(pCelHandle);
    pDialog->nHeight = D2CMP_CelGetHeight(pCelHandle);
    pDialog->nAnchorX = nPosX;
    pDialog->nAnchorY = nPosY;
    pDialog->nPosX = nPosX;
    pDialog->nPosY = nPosY;

    g_bDialogOpen = 1;
    DIALOG_BindMenuCancelHotkeys();
    WALL2_RegisterMsgHandlerTable(14, WINDOW_GetWindow(), s_aDialogMsgHandlers);
    if (GAME_HasMouseWheelB()) {
        WALL2_RegisterMsgHandlerTable(1, WINDOW_GetWindow(), s_aDialogWheelHandlers);
    }
    g_nDialogModalRefCount++;
    if (g_nActiveDialogCount != 1) {
        FOG_DisplayHalt("%s", Fog_10265(0x3CE));
        exit(-1);
    }
    return pDialog;
}

// 1.12a: 0x6FB13AA0 (__stdcall)
void __stdcall DIALOG_OnReturnKey(void* pMsg)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    if (pDialog->dwFrameCount < 1 || DIALOG_IsIgnoredMenuHotkey(pDialog, VK_RETURN)) {
        return;
    }
    if (pDialog->nSelectedIndex >= 0 && pDialog->nSelectedIndex < pDialog->nLineCount) {
        auto pfnAction = pDialog->aLines[pDialog->nSelectedIndex].pfnAction;
        if (pfnAction && pfnAction() == 1) {
            UI_StartPanelCloseClickCooldown();
        }
    } else {
        DIALOG_SelectFirstSelectableLine(pDialog);
    }
}

// 1.12a: 0x6FB13B40 (register: eax=pDialog, stack: pMsg)
int32_t __stdcall DIALOG_UpdateHoveredLine(D2DialogStrc* pDialog, void* pMsg)
{
    if (!pDialog) {
        return 0;
    }
    int nPrevSelected = pDialog->nSelectedIndex;
    int nHitLine = DIALOG_HitTestSelectableLine(pDialog, pMsg);
    pDialog->nHoveredIndex = nHitLine;
    if (nHitLine != -1) {
        pDialog->nSelectedIndex = nHitLine;
    }
    if (nHitLine != nPrevSelected && nHitLine != -1) {
        SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
        return 1;
    }
    return 0;
}

// 1.12a: 0x6FB13BB0 (register: edi=pDialog, ecx=nDelta)
void __fastcall DIALOG_StepSelection(D2DialogStrc* pDialog, int nDelta)
{
    if (pDialog->nSelectableLineCount <= 1) {
        return;
    }
    int nCur = pDialog->nSelectedIndex;
    do {
        nCur += nDelta;
        if (nCur >= pDialog->nLineCount) {
            nCur = 0;
        } else if (nCur < 0) {
            nCur = pDialog->nLineCount - 1;
        }
    } while (!pDialog->aLines[nCur].bSelectable);

    SOUND_PlaySoundEffect(1, nullptr, 0, 0, 0);
    pDialog->nSelectedIndex = nCur;
}

// 1.12a: 0x6FB13C00 (__stdcall)
void __stdcall DIALOG_OnCharKey(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnKeyDown(pNode->pWidget, pMsg)) {
            return;
        }
    }
    int nKey = static_cast<int32_t>(static_cast<D2DialogMsgStrc*>(pMsg)->wParam);
    if (DIALOG_IsIgnoredMenuHotkey(pDialog, nKey)) {
        return;
    }
    if (nKey == VK_RETURN) {
        DIALOG_OnReturnKey(pMsg);
    } else if (nKey == VK_ESCAPE) {
        DIALOG_OnEscapeKey(pMsg);
    } else if (pDialog->bBlockUnhandledKeys == 1) {
        static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
        static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
    }
}

// 1.12a: 0x6FB13CB0 (__stdcall)
void __stdcall DIALOG_OnDownArrow(void* pMsg)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnDownArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    DIALOG_StepSelection(pDialog, 1);
}

// 1.12a: 0x6FB13D30 (__stdcall)
void __stdcall DIALOG_OnUpArrow(void* pMsg)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnUpArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    DIALOG_StepSelection(pDialog, -1);
}

// 1.12a: 0x6FB13DB0 (__stdcall)
void __stdcall DIALOG_OnLeftArrow(void* pMsg)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnRightArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    DIALOG_StepSelection(pDialog, 1);
}

// 1.12a: 0x6FB13E30 (__stdcall)
void __stdcall DIALOG_OnRightArrow(void* pMsg)
{
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLeftArrow(pNode->pWidget, pMsg) == 1) {
            return;
        }
    }
    DIALOG_StepSelection(pDialog, -1);
}

// 1.12a: 0x6FB13EB0 (__stdcall)
void __stdcall DIALOG_OnMouseWheel(void* pMsg)
{
    int16_t nWheelDelta = static_cast<D2DialogMsgStrc*>(pMsg)->nWheelDelta;
    int nSteps = nWheelDelta / 120;
    if (nWheelDelta != 0 && nSteps != 0) {
        D2DialogStrc* pDialog = g_pActiveDialog;
        if (!pDialog) {
            return;
        }
        for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
            if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnMouseWheel(pNode->pWidget, pMsg) == 1) {
                return;
            }
        }
        DIALOG_StepSelection(pDialog, nSteps > 0 ? -1 : 1);
    }
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB13F60 (__stdcall)
void __stdcall DIALOG_OnMouseButtonDown(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog) {
        return;
    }
    if (pDialog->dwCreationTick + 80 > GetTickCount()) {
        static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
        static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnLButtonDown(pNode->pWidget, pMsg) == 1) {
            static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
            static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    pDialog->bMousePressedInDialog = 1;
    if (DIALOG_UpdateHoveredLine(pDialog, pMsg)) {
        static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
        static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    int nMouseX = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseX;
    int nMouseY = static_cast<D2DialogMsgStrc*>(pMsg)->wMouseY;
    if (nMouseX >= pDialog->nPosX - 4 && nMouseX <= pDialog->nPosX + pDialog->nWidth + 4 &&
        nMouseY >= pDialog->nPosY - 4 && nMouseY <= pDialog->nPosY + pDialog->nHeight + 4) {
        static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
        static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
        SMsgBreakHandlerChain(pMsg);
        return;
    }
    if (gaPanelStates[D2PANEL_MINIPANEL] && D2Client_HitTestMiniPanel(nMouseX)) {
        return;
    }
    if (pDialog->pfnOutsideClick) {
        if (!pDialog->pfnOutsideClick()) {
            return;
        }
        if (gaPanelStates[D2PANEL_NEWSTATS] && UI_HitTestNewStatBtn640(nMouseX, gnViewportShiftMode, nMouseY)) {
            static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
            static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
        }
        if (gaPanelStates[D2PANEL_NEWSKILLS] && UI_HitTestNewSkillBtn640(nMouseX, gnViewportShiftMode, nMouseY)) {
            static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
            static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
        }
        return;
    }
    if (!pDialog->pfnCancel) {
        FOG_DisplayHalt("%s", Fog_10265(0x1AA));
        exit(-1);
    }
    pDialog->pfnCancel();
    static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
    static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
    SMsgBreakHandlerChain(pMsg);
}

// 1.12a: 0x6FB14120 (__stdcall)
void __stdcall DIALOG_OnMouseMove(void* pMsg)
{
    D2DialogStrc* pDialog = g_pActiveDialog;
    if (!pDialog || pDialog->dwCreationTick + 80 > GetTickCount()) {
        return;
    }
    for (D2DialogWidgetNodeStrc* pNode = pDialog->pWidgetList; pNode; pNode = pNode->pNext) {
        if (pNode->pWidget && pNode->pWidget->pVtbl->pfnOnMouseMove(pNode->pWidget, pMsg) == 1) {
            static_cast<D2DialogMsgStrc*>(pMsg)->bHandled = 1;
            static_cast<D2DialogMsgStrc*>(pMsg)->lResult = 0;
            SMsgBreakHandlerChain(pMsg);
            return;
        }
    }
    DIALOG_UpdateHoveredLine(pDialog, pMsg);
}


// Legacy cross-TU aliases (UI_useitem.cpp & UI_options.cpp)
void* __stdcall DIALOG_Create(int a0, int a1, void* a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    return DIALOG_CreateFramed(a0, a1, reinterpret_cast<int (__cdecl*)(void)>(a2), reinterpret_cast<int (__cdecl*)(void)>(a3), a4, a5, a6, a7, a8, a9, a10);
}
D2DialogStrc* __stdcall DIALOG_CreateDynamic(int a0, int a1, int (__cdecl* a2)(void), int (__cdecl* a3)(void), int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    return DIALOG_CreateFramed(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}
void __stdcall DIALOG_AddLine(D2DialogStrc* pDialog, int nColor, const Unicode* wszText, int nLineHeight, int nUnkF4, int nFont, int (__cdecl* pfnAction)(void), int bSelectable)
{
    DIALOG_AppendLine(pDialog, nColor, wszText, nLineHeight, nUnkF4, nFont, pfnAction, bSelectable);
}
void __cdecl DIALOG_CloseActiveDialog(void) { DIALOG_CloseActive(); }
void __fastcall DIALOG_Destroy(void* pDlg) { (void)pDlg; DIALOG_CloseActive(); }
void __fastcall DIALOG_Draw(void* pDlg) { if (pDlg) DIALOG_Render(static_cast<D2DialogStrc*>(pDlg)); }
void __fastcall DIALOG_SetLineText(void* pDlg, int nColor, const Unicode* wszText, int nLineHeight, int nUnkF4, int nFont, int pfnAction, int bSelectable)
{
    if (pDlg) DIALOG_AppendLine(static_cast<D2DialogStrc*>(pDlg), nColor, wszText, nLineHeight, nUnkF4, nFont, reinterpret_cast<int (__cdecl*)(void)>(pfnAction), bSelectable);
}
