// ============================================================================
// UI_text.cpp
// Reconstructed from Diablo II 1.12a D2Client.dll
// Address ranges:
//   0x6FB1F3C0..0x6FB26790 (asm/UI_text.asm   - 128 functions)
//   0x6FB34180..0x6FB34E90 (asm/UI_editbox.asm - 42 functions)
//   0x6FB34E90..0x6FB373F0 (asm/UI_chat.asm    - 30 functions)
// ============================================================================

#include <windows.h>
#include <imm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Forward Declarations & Core Engine Types
// ----------------------------------------------------------------------------
struct D2UnitStrc;
struct D2CellFileStrc;
struct D2GfxDataStrc;
struct D2HoverTextStrc;
struct D2MsgStrc;
typedef uint16_t Unicode;

struct D2MsgHandlerStrc
{
    int32_t nType;
    uint32_t nMsg;
    void (__stdcall *pfnHandler)(D2MsgStrc* pMsg);
};

#pragma pack(push, 1)
struct D2ChatMsgNodeStrc
{
    Unicode wszText[258];           // +0x000
    uint32_t dwStartTick;           // +0x204
    uint32_t dwDuration;            // +0x208
    uint8_t nColor;                 // +0x20C
    uint8_t nPad[3];                // +0x20D
    D2ChatMsgNodeStrc* pPrev;       // +0x210
    D2ChatMsgNodeStrc* pNext;       // +0x214
};

struct D2ChatMsgListStrc
{
    int16_t nCount;                 // +0x00
    D2ChatMsgNodeStrc* pHead;       // +0x02
};

struct D2ConfigKeyEntryStrc
{
    int32_t nActionId;              // +0x00
    uint16_t wStringId;             // +0x04
    uint32_t dwReserved;            // +0x06
};

struct D2ConfigBtnStrc
{
    uint16_t wStringId;             // +0x00
    void (__stdcall *pfnCallback)();// +0x02
    int32_t nColor;                 // +0x06
    int32_t nLeft;                  // +0x0A
    int32_t nRight;                 // +0x0E
    int32_t nTop;                   // +0x12
    int32_t nBottom;                // +0x16
};

struct D2NpcSpeechBoxStrc
{
    int32_t nX;                     // +0x00
    int32_t nY;                     // +0x04
    int32_t nWidth;                 // +0x08
    int32_t nHeight;                // +0x0C
    uint8_t nPad10[6];              // +0x10
    int32_t nState;                 // +0x16
    int32_t nLineCount;             // +0x1A
    int32_t nTotalTextHeight;       // +0x1E
    int32_t nVisibleLines;          // +0x22
    int32_t nMaxScrollOffset;       // +0x26
    int32_t nScrollSpeed;           // +0x2A
    uint32_t dwLastTick;            // +0x2E
    int32_t bHasAudio;              // +0x32
    int32_t nSoundHandle;           // +0x36
    int32_t nTopLineIndex;          // +0x3A
    int32_t nLinePixelOffset;       // +0x3E
    int32_t bPaused;                // +0x42
    int32_t nLastMouseY;            // +0x46
    Unicode* pLines[64];            // +0x4A
    int32_t nLineWidths[64];        // +0x14A
};
#pragma pack(pop)

// ============================================================================
// PART 1: UI_text.asm (0x6FB1F3C0..0x6FB26790 - 128 functions)
// Message log, overhead chat bubbles, NPC scrolling speech box, Scroll of
// Inifuss (Cain rune puzzle), S2C 0x26 chat & S2C 0x5A event message formatting,
// and Configure Controls UI panel.
// ============================================================================

// 1.12a: 0x6FB1F3C0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F3C0()
{
    // Unlinks and frees the tail node of the screen chat message list (0x6FBB54CC).
}

// 1.12a: 0x6FB1F460
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F460()
{
    // Unlinks and frees the tail node of the system event message list (0x6FBB54D2).
}

// 1.12a: 0x6FB1F500
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F500()
{
    // Unlinks and frees the tail node of the message log history list (0x6FBB54D8).
}

// 1.12a: 0x6FB1F5A0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F5A0()
{
    // Resets the active overhead chat bubble bounding-box count (0x6FBB5488 = 0).
    *(int32_t*)0x6FBB5488 = 0;
}

// 1.12a: 0x6FB1F5B0
// Calling convention: __fastcall / register (eax = RECT* pRect)
void __fastcall sub_6FB1F5B0(RECT* pRect)
{
    // Appends a bounding rectangle to the overhead chat bubble collision array (0x6FBB5388, max 16).
    int32_t nCount = *(int32_t*)0x6FBB5488;
    if (nCount < 16)
    {
        RECT* pDst = &((RECT*)0x6FBB5388)[nCount];
        *pDst = *pRect;
        *(int32_t*)0x6FBB5488 = nCount + 1;
    }
}

// 1.12a: 0x6FB1F5E0
// Calling convention: __fastcall / register (eax = RECT* pRect)
BOOL __fastcall sub_6FB1F5E0(RECT* pRect)
{
    // Tests whether pRect intersects any existing overhead chat bubble rectangle in 0x6FBB5388[0..nCount].
    RECT rcIntersect;
    int32_t nCount = *(int32_t*)0x6FBB5488;
    for (int32_t i = 0; i < nCount; ++i)
    {
        if (IntersectRect(&rcIntersect, pRect, &((RECT*)0x6FBB5388)[i]))
            return TRUE;
    }
    return FALSE;
}

// 1.12a: 0x6FB1F630
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F630()
{
    // Returns 1 if the NPC speech box is active (0x6FBB54A8 != 0), else 0.
    return *(int32_t*)0x6FBB54A8 != 0;
}

// 1.12a: 0x6FB1F640
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit)
BOOL __fastcall sub_6FB1F640(D2UnitStrc* pUnit)
{
    // Returns TRUE if the unit has an active overhead hover text that is non-empty.
    return FALSE;
}

// 1.12a: 0x6FB1F670
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F670()
{
    // Returns the active NPC interaction unit GUID (0x6FBB54EA) if 0x6FBB54E2 != 0, else -1.
    return (*(int32_t*)0x6FBB54E2 != 0) ? *(int32_t*)0x6FBB54EA : -1;
}

// 1.12a: 0x6FB1F690
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F690()
{
    // Returns the active NPC interaction unit type (0x6FBB54E6) if 0x6FBB54E2 != 0, else 6.
    return (*(int32_t*)0x6FBB54E2 != 0) ? *(int32_t*)0x6FBB54E6 : 6;
}

// 1.12a: 0x6FB1F6B0
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F6B0()
{
    // Returns 0x6FBB54E2 (whether an NPC interaction is active).
    return *(int32_t*)0x6FBB54E2;
}

// 1.12a: 0x6FB1F6C0
// Calling convention: __fastcall (ecx = int32_t bActive, edx = int32_t nUnitType, arg0 = int32_t nUnitGUID)
void __fastcall sub_6FB1F6C0(int32_t bActive, int32_t nUnitType, int32_t nUnitGUID)
{
    // Sets active NPC interaction state (0x6FBB54E2, 0x6FBB54E6, 0x6FBB54EA).
    *(int32_t*)0x6FBB54E2 = bActive;
    *(int32_t*)0x6FBB54E6 = nUnitType;
    *(int32_t*)0x6FBB54EA = nUnitGUID;
}

// 1.12a: 0x6FB1F6E0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F6E0()
{
    // Clears 0x6FBB54EE = 0.
    *(int32_t*)0x6FBB54EE = 0;
}

// 1.12a: 0x6FB1F6F0
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F6F0()
{
    // Returns 0x6FBB54EE.
    return *(int32_t*)0x6FBB54EE;
}

// 1.12a: 0x6FB1F700
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F700()
{
    // Returns 0x6FBB54A0.
    return *(int32_t*)0x6FBB54A0;
}

// 1.12a: 0x6FB1F710
// Calling convention: __stdcall (int32_t nVal)
void __stdcall sub_6FB1F710(int32_t nVal)
{
    // Sets 0x6FBB54A0 = nVal.
    *(int32_t*)0x6FBB54A0 = nVal;
}

// 1.12a: 0x6FB1F720
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F720()
{
    // Sets 0x6FBB5498 = 1 (Requests NPC menu open).
    *(int32_t*)0x6FBB5498 = 1;
}

// 1.12a: 0x6FB1F730
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F730()
{
    // Returns 0x6FBB5498.
    return *(int32_t*)0x6FBB5498;
}

// 1.12a: 0x6FB1F740
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F740()
{
    // Returns 0x6FBB549C.
    return *(int32_t*)0x6FBB549C;
}

// 1.12a: 0x6FB1F750
// Calling convention: __stdcall (int32_t nVal)
void __stdcall sub_6FB1F750(int32_t nVal)
{
    // Sets 0x6FBB549C = nVal.
    *(int32_t*)0x6FBB549C = nVal;
}

// 1.12a: 0x6FB1F760
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F760()
{
    // Returns 0x6FBB550C (NPC speech box rect valid flag).
    return *(int32_t*)0x6FBB550C;
}

// 1.12a: 0x6FB1F770
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F770()
{
    // Clears 0x6FBB550C = 0.
    *(int32_t*)0x6FBB550C = 0;
}

// 1.12a: 0x6FB1F780
// Calling convention: __stdcall (RECT* pRect)
void __stdcall sub_6FB1F780(RECT* pRect)
{
    // Copies the screen bounds of the NPC speech box (0x6FBB5388) into *pRect.
    *pRect = *(RECT*)0x6FBB5388;
}

// 1.12a: 0x6FB1F7B0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F7B0()
{
    // Frees the Scroll of Inifuss / barkscroll CEL asset (0x6FBB54B0) and resets 0x6FBB54AC.
}

// 1.12a: 0x6FB1F800
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F800()
{
    // Clears the message log scroll offset (0x6FBB5538 = 0).
    *(int32_t*)0x6FBB5538 = 0;
}

// 1.12a: 0x6FB1F810
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F810()
{
    // Scrolls the message log up by 4 lines, clamped to (nLineCount - 16).
}

// 1.12a: 0x6FB1F840
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F840()
{
    // Scrolls the message log down by 4 lines, clamped to 0.
}

// 1.12a: 0x6FB1F860
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F860()
{
    // Frees all nodes in the message log history list (0x6FBB54D8) and resets scroll offset.
}

// 1.12a: 0x6FB1F880
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F880()
{
    // Returns 0x6FBB553C (Trade/Stash/Cube background open state).
    return *(int32_t*)0x6FBB553C;
}

// 1.12a: 0x6FB1F890
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F890()
{
    // Clears 0x6FBB553C = 0.
    *(int32_t*)0x6FBB553C = 0;
}

// 1.12a: 0x6FB1F8A0
// Calling convention: __fastcall / register (esi = D2NpcSpeechBoxStrc* pBox)
void __fastcall sub_6FB1F8A0(D2NpcSpeechBoxStrc* pBox)
{
    // Handles mouse dragging of the NPC speech box text area to scroll up/down.
}

// 1.12a: 0x6FB1F8E0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB1F8E0(D2MsgStrc* pMsg)
{
    // WM_RBUTTONUP handler during NPC speech: breaks handler chain if 0x6FBB5527 is set.
}

// 1.12a: 0x6FB1F910
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F910()
{
    // Clears 0x6FBB5498 = 0 and 0x6FBB549C = 0, and closes open NPC dialog/menu.
}

// 1.12a: 0x6FB1FA70
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1FA70()
{
    // Closes the vendor/trade panel, frees 0x6FBB5494, and sends C2S 0x30 if talking to an NPC.
}

// 1.12a: 0x6FB1FB00
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit)
void __fastcall sub_6FB1FB00(D2UnitStrc* pUnit)
{
    // Frees any existing overhead hover message on pUnit (+0xA0) via D2COMMON_10891_CHAT_FreeHoverMsg.
}

// 1.12a: 0x6FB1FB40
// Calling convention: __fastcall / register (edi = D2UnitStrc* pUnit, edx = const char* szText, arg0 = int32_t nLang)
void __fastcall sub_6FB1FB40(D2UnitStrc* pUnit, const char* szText, int32_t nLang)
{
    // Allocates and attaches an overhead hover message to pUnit via D2COMMON_10890_CHAT_AllocHoverMsg,
    // and adds pUnit to the active overhead bubble unit list (0x6FBB5514).
}

// 1.12a: 0x6FB1FC50
// Calling convention: __fastcall / register (eax = Unicode* pOut, edx = const char* szIn, edi = int32_t nLang)
uint8_t __fastcall sub_6FB1FC50(Unicode* pOut, const char* szIn, int32_t nLang)
{
    // Converts a network chat string to Unicode (using MultiByteToWideChar for CJK or Unicode::win2Unicode).
    return 1;
}

// 1.12a: 0x6FB1FCE0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1FCE0()
{
    // Draws active system event messages from 0x6FBB54D2 in the upper-left area of the viewport,
    // expiring nodes whose duration has elapsed.
}

// 1.12a: 0x6FB1FE90
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1FE90()
{
    // Draws active player chat messages from 0x6FBB54CC in the upper-left area of the viewport,
    // expiring nodes whose duration has elapsed.
}

// 1.12a: 0x6FB20050
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20050()
{
    // Draws the full-screen Message Log panel (0x6FBB54D8) with scrollbar and timestamps.
}

// 1.12a: 0x6FB202F0
// Calling convention: __stdcall (D2NpcSpeechBoxStrc* pBox)
BOOL __stdcall sub_6FB202F0(D2NpcSpeechBoxStrc* pBox)
{
    // Updates and renders the NPC scrolling speech box, syncing scroll speed with speech audio playback.
    return TRUE;
}

// 1.12a: 0x6FB206F0
// Calling convention: __fastcall / register (esi = D2NpcSpeechBoxStrc* pBox)
void __fastcall sub_6FB206F0(D2NpcSpeechBoxStrc* pBox)
{
    // Stops any playing NPC speech audio, frees the line buffer (pBox->pLines[0]), and frees pBox.
}

// 1.12a: 0x6FB20760
// Calling convention: __fastcall (ecx = const Unicode* wszText, edx = int32_t nSoundId, arg0..arg5 = x, y, w, h, nSpeed, bPlayAudio)
D2NpcSpeechBoxStrc* __fastcall sub_6FB20760(
    const Unicode* wszText,
    int32_t nSoundId,
    int32_t nX,
    int32_t nY,
    int32_t nWidth,
    int32_t nHeight,
    int32_t nScrollSpeed,
    BOOL bPlayAudio)
{
    // Allocates and word-wraps a D2NpcSpeechBoxStrc (sizeof = 0x24A) and starts speech audio if enabled.
    return NULL;
}

// 1.12a: 0x6FB20AF0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20AF0()
{
    // Frees all nodes in 0x6FBB54CC, 0x6FBB54D2, and 0x6FBB54D8.
}

// 1.12a: 0x6FB20B10
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20B10()
{
    // Closes the Message Log panel, restores view matrix adjust, and unregisters message log handlers.
}

// 1.12a: 0x6FB20C00
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20C00()
{
    // Closes the Scroll of Inifuss / barkscroll display and unregisters Escape/Space handlers.
}

// 1.12a: 0x6FB20C40
// Calling convention: __fastcall / register (ecx = int32_t nMode, esi = D2GfxDataStrc* pGfxData, edi = const Unicode* wszText)
void __fastcall sub_6FB20C40(int32_t nMode, D2GfxDataStrc* pGfxData, const Unicode* wszText)
{
    // Draws the Scroll of Inifuss / barkscroll background and either wrapped text or Cairn Stone rune symbols.
}

// 1.12a: 0x6FB20D90
// Calling convention: __fastcall (ecx = const Unicode* wszText, edx = int32_t nSoundId, arg0 = int32_t nScrollSpeed, arg1 = BOOL bPlayAudio)
D2NpcSpeechBoxStrc* __fastcall sub_6FB20D90(const Unicode* wszText, int32_t nSoundId, int32_t nScrollSpeed, BOOL bPlayAudio)
{
    // Wrapper around sub_6FB20760 using standard centered NPC speech box dimensions.
    return sub_6FB20760(
        wszText,
        nSoundId,
        *(int32_t*)0x6FBD3304,
        *(int32_t*)0x6FB928EC,
        325,
        112,
        nScrollSpeed,
        bPlayAudio);
}

// 1.12a: 0x6FB20DD0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20DD0()
{
    // Frees the active NPC speech box (0x6FBB5490) and resets 0x6FBCC120.
}

// 1.12a: 0x6FB20DF0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB20DF0(D2MsgStrc* pMsg)
{
    // Escape / Space key handler while Message Log is open: closes Message Log and breaks chain.
}

// 1.12a: 0x6FB20E30
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20E30()
{
    // Opens the Message Log panel, adjusts viewport matrix, and registers Escape/Space handlers.
}

// 1.12a: 0x6FB20F40
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20F40()
{
    // Cleanup helper called on game leave: frees message lists, barkscroll, speech box, and vendor state.
}

// 1.12a: 0x6FB20FB0
// Calling convention: __fastcall / register (eax = uint8_t* pPacket)
void __fastcall sub_6FB20FB0(uint8_t* pPacket)
{
    // Handles Cairn Stone order packet: loads UI\Game\scroin, scroin2, scroin3 and displays the 5 rune order.
}

// 1.12a: 0x6FB21170
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB21170()
{
    // Closes active NPC speech box (0x6FBB5490), resets state flags, and unregisters NPC speech handlers.
}

// 1.12a: 0x6FB21340
// Calling convention: __stdcall (int32_t nUnitType, int32_t nUnitGUID, uint16_t wStringId, BOOL bMenuAfter)
D2NpcSpeechBoxStrc* __stdcall sub_6FB21340(int32_t nUnitType, int32_t nUnitGUID, uint16_t wStringId, BOOL bMenuAfter)
{
    // Starts an NPC speech dialog for wStringId, registering mouse/key handlers (0x6FB928F0).
    return NULL;
}

// 1.12a: 0x6FB21460
// Calling convention: __fastcall (ecx = const Unicode* wszText, edx = uint8_t nColor)
void __fastcall sub_6FB21460(const Unicode* wszText, uint8_t nColor)
{
    // Appends a message to the Message Log history list (0x6FBB54D8, max 128 entries).
}

// 1.12a: 0x6FB215B0
// Calling convention: __stdcall (const Unicode* wszText, uint8_t nColor)
void __stdcall sub_6FB215B0(const Unicode* wszText, uint8_t nColor)
{
    // Word-wraps and prepends a message to the upper-left system event list (0x6FBB54D2, max 3 entries).
}

// 1.12a: 0x6FB21740
// Calling convention: __stdcall (const Unicode* wszText, uint8_t nColor)
void __stdcall sub_6FB21740(const Unicode* wszText, uint8_t nColor)
{
    // Word-wraps and prepends a chat message to the screen chat list (0x6FBB54CC, max 8 entries),
    // plays chat alert sound (4), and records the message in the Message Log (sub_6FB21460).
}

// 1.12a: 0x6FB21990
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB21990()
{
    // Iterates active overhead chat bubble units (0x6FBB5514) and expires timed-out hover messages.
}

// 1.12a: 0x6FB21AD0
// Calling convention: __stdcall (int32_t nX, int32_t nY)
void __stdcall sub_6FB21AD0(int32_t nX, int32_t nY)
{
    // Renders the Horadric Scroll / Scroll of Inifuss barkscroll popup at (nX, nY).
}

// 1.12a: 0x6FB21ED0
// Calling convention: __stdcall (uint16_t wStringId, int32_t nColor)
void __stdcall sub_6FB21ED0(uint16_t wStringId, int32_t nColor)
{
    // Formats a localized string ID with the local player's name and adds it to the screen chat list.
}

// 1.12a: 0x6FB22000
// Calling convention: __stdcall (const char* szText, uint8_t nColor)
void __stdcall sub_6FB22000(const char* szText, uint8_t nColor)
{
    // Converts ASCII szText to Unicode and adds it to the screen chat list (sub_6FB21740).
}

// 1.12a: 0x6FB220E0
// Calling convention: __stdcall (const char* szText, uint8_t nColor)
void __stdcall sub_6FB220E0(const char* szText, uint8_t nColor)
{
    // Converts ASCII szText to Unicode and adds it to the system event message list (sub_6FB215B0).
}

// 1.12a: 0x6FB221C0
// Calling convention: __fastcall (ecx = const char* szPlayerName, edx = uint16_t wStringId, arg0 = uint8_t nColor)
void __fastcall sub_6FB221C0(const char* szPlayerName, uint16_t wStringId, uint8_t nColor)
{
    // Formats "<szPlayerName> <String(wStringId)>" and adds it to the system event message list.
}

// 1.12a: 0x6FB22360
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit)
void __fastcall sub_6FB22360(D2UnitStrc* pUnit)
{
    // Renders the overhead chat bubble above a monster/object unit (pUnit) if it does not collide.
}

// 1.12a: 0x6FB224C0
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit)
void __fastcall sub_6FB224C0(D2UnitStrc* pUnit)
{
    // Renders the overhead chat bubble above a player unit (pUnit), drawing a speech balloon frame.
}

// 1.12a: 0x6FB226C0
// Calling convention: __stdcall (uint16_t wStringId)
void __stdcall sub_6FB226C0(uint16_t wStringId)
{
    // Opens the barkscroll popup with the localized text for wStringId.
}

// 1.12a: 0x6FB22710
// Calling convention: __stdcall (uint8_t nEventType, uint8_t nColor, uint32_t dwArg, uint8_t nArg, const char* szName1, const char* szName2)
void __stdcall sub_6FB22710(
    uint8_t nEventType,
    uint8_t nColor,
    uint32_t dwArg,
    uint8_t nArg,
    const char* szName1,
    const char* szName2)
{
    // Handles S2C 0x5A event notifications (dropped due to timeout, joined/left game, slain by,
    // Diablo walks the earth, Stones of Jordan sold, party/hostile relations, whisper notifications).
}

// 1.12a: 0x6FB22E40
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB22E40()
{
    // Cancels or advances the active NPC speech box or barkscroll popup when clicked or dismissed.
    return FALSE;
}

// 1.12a: 0x6FB22FA0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB22FA0(D2MsgStrc* pMsg)
{
    // Key/mouse dismissal handler for NPC speech box / barkscroll.
}

// 1.12a: 0x6FB23000
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB23000(D2MsgStrc* pMsg)
{
    // Checks if key matches bound Action 7 (Quest Log) before forwarding to sub_6FB22FA0.
}

// 1.12a: 0x6FB23070
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB23070(D2MsgStrc* pMsg)
{
    // Ignores VK_F4 (0x73), otherwise forwards to sub_6FB23000.
}

// 1.12a: 0x6FB23090
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB23090()
{
    // Transitions from finished NPC speech to opening the NPC menu (0x6FACA970) or closing dialog.
}

// 1.12a: 0x6FB230D0
// Calling convention: __fastcall (ecx = D2NpcSpeechBoxStrc* pBox, eax = BOOL bAbort)
int32_t __fastcall sub_6FB230D0(D2NpcSpeechBoxStrc* pBox, BOOL bAbort)
{
    // Advances NPC speech box state; returns 1 while still speaking, 0 when finished.
    return 0;
}

// 1.12a: 0x6FB231E0
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB231E0()
{
    // Thunk to sub_6FB22E40.
    return sub_6FB22E40();
}

// 1.12a: 0x6FB231F0
// Calling convention: __fastcall / register (eax = void* pView)
void __fastcall sub_6FB231F0(void* pView)
{
    // Main per-frame text & dialog render pass: draws player/monster/object overhead bubbles,
    // updates NPC speech box & barkscroll, renders screen chat & system messages, and opens NPC menu.
}

// 1.12a: 0x6FB23490
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB23490(D2MsgStrc* pMsg)
{
    // WM_LBUTTONDOWN / WM_RBUTTONDOWN handler during NPC speech.
}

// 1.12a: 0x6FB235B0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB235B0()
{
    // Toggles the Message Log panel open/closed when no NPC speech box is active.
}

// 1.12a: 0x6FB23700
// Calling convention: __userpurge (eax = D2UnitStrc* pNpc, arg0 = D2UnitStrc* pInteractUnit)
void __stdcall sub_6FB23700(D2UnitStrc* pInteractUnit)
{
    // Opens the NPC trade / stash / Horadric Cube inventory UI and adjusts viewport matrix.
}

// 1.12a: 0x6FB239C0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB239C0(D2MsgStrc* pMsg)
{
    // Close button handler for the left-side NPC trade / stash / cube panel.
}

// 1.12a: 0x6FB23C80
// Calling convention: __fastcall / register (eax = D2MsgStrc* pMsg)
void __fastcall sub_6FB23C80(D2MsgStrc* pMsg)
{
    // Escape key handler for the left-side NPC trade / stash / cube panel.
}

// 1.12a: 0x6FB23EE0
// Calling convention: __stdcall (uint8_t* pPacket)
void __stdcall sub_6FB23EE0(uint8_t* pPacket)
{
    // S2C 0x26 chat packet handler: formats normal chat, whispers, overhead bubbles, and scroll text.
}

// 1.12a: 0x6FB24350
// Calling convention: __fastcall (ecx = int32_t nControlIdx)
BOOL __fastcall sub_6FB24350(int32_t nControlIdx)
{
    // Returns TRUE if no other control in the Configure Controls screen is currently captured.
    for (int32_t i = 0; i < 7; ++i)
    {
        if (((int32_t*)0x6FBCC0B0)[i] != 0 && i != nControlIdx)
            return FALSE;
    }
    return TRUE;
}

// 1.12a: 0x6FB243D0
// Calling convention: __fastcall / register (esi = int32_t nMouseX, edi = int32_t nMouseY)
BOOL __fastcall sub_6FB243D0(int32_t nMouseX, int32_t nMouseY)
{
    // Hit-tests the scrollbar thumb in the Configure Controls panel.
    return FALSE;
}

// 1.12a: 0x6FB24420
// Calling convention: __fastcall / register (esi = int32_t nMouseX, edi = int32_t nMouseY)
BOOL __fastcall sub_6FB24420(int32_t nMouseX, int32_t nMouseY)
{
    // Hit-tests the upper scrollbar track (page up region) in the Configure Controls panel.
    return FALSE;
}

// 1.12a: 0x6FB24470
// Calling convention: __fastcall / register (esi = int32_t nMouseX, edi = int32_t nMouseY)
BOOL __fastcall sub_6FB24470(int32_t nMouseX, int32_t nMouseY)
{
    // Hit-tests the scrollbar down arrow button in the Configure Controls panel.
    return FALSE;
}

// 1.12a: 0x6FB24510
// Calling convention: __fastcall / register (edi = int32_t nMouseX, ebx = int32_t nMouseY)
BOOL __fastcall sub_6FB24510(int32_t nMouseX, int32_t nMouseY)
{
    // Hit-tests the keybinding list area in the Configure Controls panel.
    return FALSE;
}

// 1.12a: 0x6FB24580
// Calling convention: __fastcall / register (esi = int32_t nMouseX, edx = int32_t nMouseY)
int32_t __fastcall sub_6FB24580(int32_t nMouseX, int32_t nMouseY)
{
    // Hit-tests the 3 bottom buttons (Accept, Default, Cancel) in Configure Controls; returns 0..2 or 3.
    return 3;
}

// 1.12a: 0x6FB245B0
// Calling convention: __stdcall (int32_t nMouseX)
BOOL __stdcall sub_6FB245B0(int32_t nMouseX)
{
    // Returns 1 if nMouseX is in the Primary key column, 0 if in the Secondary key column.
    return TRUE;
}

// 1.12a: 0x6FB245F0
// Calling convention: __fastcall / register (ebx = int32_t nMouseY)
int32_t __fastcall sub_6FB245F0(int32_t nMouseY)
{
    // Converts a mouse Y coordinate into a row index in the Configure Controls keybinding list.
    return 0;
}

// 1.12a: 0x6FB24640
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB24640()
{
    // Returns the maximum top row scroll index (nTotalRows - 15) in Configure Controls.
    int32_t nCount = *(int32_t*)0x6FBCC09C;
    for (int32_t i = 0; i < 15; ++i)
    {
        if (--nCount <= 0)
            return 0;
    }
    return nCount;
}

// 1.12a: 0x6FB24680
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB24680()
{
    // Frees the TextSlid CEL asset (0x6FBCC0E4) used by the Configure Controls scrollbar.
}

// 1.12a: 0x6FB246C0
// Calling convention: __fastcall / register (esi = int32_t nMouseX, edx = int32_t nMouseY)
BOOL __fastcall sub_6FB246C0(int32_t nMouseX, int32_t nMouseY)
{
    // Returns TRUE if the mouse is still over the currently pressed Configure Controls button.
    return FALSE;
}

// 1.12a: 0x6FB24750
// Calling convention: __fastcall (ecx = int32_t nDelta)
void __fastcall sub_6FB24750(int32_t nDelta)
{
    // Scrolls the Configure Controls list by nDelta rows and updates scrollbar thumb Y (0x6FBD3300).
}

// 1.12a: 0x6FB24810
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB24810()
{
    // Initializes Configure Controls row count and table pointer (0x6FB925B8 Expansion / 0x6FB923B8 Classic).
}

// 1.12a: 0x6FB248F0
// Calling convention: __stdcall (BOOL bRestoreHotkeys)
void __stdcall sub_6FB248F0(BOOL bRestoreHotkeys)
{
    // Unregisters key capture message handlers (0x6FB92880) when leaving key-bind capture mode.
}

// 1.12a: 0x6FB24A30
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB24A30(D2MsgStrc* pMsg)
{
    // WM_MOUSEWHEEL handler while capturing a keybinding (binds Wheel Up 0x103 / Wheel Down 0x104).
}

// 1.12a: 0x6FB24AE0
// Calling convention: __fastcall / register (eax = int32_t nVirtualKey)
void __fastcall sub_6FB24AE0(int32_t nVirtualKey)
{
    // Assigns mouse button virtual key (0x100 MButton, 0x101 XButton1, 0x102 XButton2) during capture.
}

// 1.12a: 0x6FB24B40
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB24B40()
{
    // Computes bounding rectangles for the 3 Configure Controls buttons and scrollbar thumb position.
}

// 1.12a: 0x6FB24C70
// Calling convention: __fastcall / register (eax = int32_t nRow, edi = int32_t nColumn)
int32_t __fastcall sub_6FB24C70(int32_t nRow, int32_t nColumn)
{
    // Determines text color (gold, white, or grey) for a row/column in Configure Controls.
    return 0;
}

// 1.12a: 0x6FB24CD0
// Calling convention: __fastcall (ecx = int32_t nDelta)
void __fastcall sub_6FB24CD0(int32_t nDelta)
{
    // Moves the selected row highlight (0x6FBCC0D8) up/down by nDelta and keeps it scrolled in view.
}

// 1.12a: 0x6FB24E20
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB24E20()
{
    // Cancels an in-progress keybinding capture and restores the backup keymap (0x6FBB0D28 -> 0x6FBB11A0).
}

// 1.12a: 0x6FB24EA0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB24EA0(D2MsgStrc* pMsg)
{
    // WM_XBUTTONDOWN handler during keybinding capture.
}

// 1.12a: 0x6FB24EF0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB24EF0(D2MsgStrc* pMsg)
{
    // WM_MBUTTONDOWN handler during keybinding capture.
}

// 1.12a: 0x6FB24F30
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB24F30(D2MsgStrc* pMsg)
{
    // WM_KEYUP / WM_SYSKEYUP handler during keybinding capture.
}

// 1.12a: 0x6FB24FF0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB24FF0(D2MsgStrc* pMsg)
{
    // WM_KEYDOWN / WM_SYSKEYDOWN handler during keybinding capture.
}

// 1.12a: 0x6FB250F0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB250F0(D2MsgStrc* pMsg)
{
    // Delete / Backspace key handler in Configure Controls: clears the selected keybinding.
}

// 1.12a: 0x6FB25180
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB25180(D2MsgStrc* pMsg)
{
    // Right arrow key handler in Configure Controls: selects Secondary key column (0).
}

// 1.12a: 0x6FB251E0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB251E0(D2MsgStrc* pMsg)
{
    // Left arrow key handler in Configure Controls: selects Primary key column (1).
}

// 1.12a: 0x6FB25240
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB25240(D2MsgStrc* pMsg)
{
    // Down arrow key handler in Configure Controls: moves selection to next non-separator row.
}

// 1.12a: 0x6FB25300
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB25300(D2MsgStrc* pMsg)
{
    // Up arrow key handler tail in Configure Controls: breaks handler chain after moving selection up.
}

// 1.12a: 0x6FB25320
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB25320(D2MsgStrc* pMsg)
{
    // WM_MOUSEWHEEL handler in Configure Controls list: scrolls list by 2 rows per wheel notch.
}

// 1.12a: 0x6FB25390
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB25390()
{
    // Loads UI\Menu\TextSlid CEL and initializes the Configure Controls panel state.
}

// 1.12a: 0x6FB253F0
// Calling convention: __stdcall (int32_t nMouseX, int32_t nMouseY)
void __stdcall sub_6FB253F0(int32_t nMouseX, int32_t nMouseY)
{
    // Processes mouse hover, drag, and scrollbar interaction in the Configure Controls panel.
}

// 1.12a: 0x6FB258F0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB258F0(D2MsgStrc* pMsg)
{
    // WM_LBUTTONDOWN / WM_RBUTTONDOWN handler during keybinding capture: cancels capture and forwards click.
}

// 1.12a: 0x6FB25950
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB25950(D2MsgStrc* pMsg)
{
    // WM_LBUTTONDOWN handler in Configure Controls panel.
}

// 1.12a: 0x6FB259A0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB259A0()
{
    // Renders the entire Configure Controls panel (frame, headers, 15 visible rows, scrollbar, and buttons).
}

// 1.12a: 0x6FB261C0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB261C0()
{
    // Default button callback in Configure Controls: resets keybindings to default (0x6FB513A0).
}

// 1.12a: 0x6FB261D0
// Calling convention: __stdcall (BOOL bInit)
void __stdcall sub_6FB261D0(BOOL bInit)
{
    // Enters or exits Configure Controls panel, backing up keybindings (0x6FBB11A0 -> 0x6FBB0D28).
}

// 1.12a: 0x6FB262B0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB262B0()
{
    // Starts keybinding capture mode (0x6FBCC0E0 = 1) and registers capture handlers (0x6FB92880).
}

// 1.12a: 0x6FB26420
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB26420(D2MsgStrc* pMsg)
{
    // Enter / Space key handler in Configure Controls: starts capturing a key for the selected row.
}

// 1.12a: 0x6FB264B0
// Calling convention: __userpurge (eax = int32_t nMouseY, arg0 = int32_t nMouseX)
void __stdcall sub_6FB264B0(int32_t nMouseX)
{
    // Mouse button release helper in Configure Controls: triggers row capture or bottom button callback.
}

// 1.12a: 0x6FB26570
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB26570(D2MsgStrc* pMsg)
{
    // WM_LBUTTONUP handler in Configure Controls panel.
}

// 1.12a: 0x6FB265D0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB265D0()
{
    // Cancel button callback in Configure Controls: restores backup keybindings and returns to Options menu.
}

// 1.12a: 0x6FB26680
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB26680()
{
    // Accept button callback in Configure Controls: saves keybindings and returns to Options menu.
}

// 1.12a: 0x6FB26720
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB26720(D2MsgStrc* pMsg)
{
    // Escape key handler in Configure Controls: invokes Cancel (sub_6FB265D0).
}

// ============================================================================
// PART 2: UI_editbox.asm (0x6FB34180..0x6FB34E90 - 42 functions)
// CEditBoxProxy (sizeof = 0x0C, vtable 0x6FB846F8) and
// CEditBoxImpl  (sizeof = 0x5A, vtable 0x6FB84748)
// ============================================================================

#pragma pack(push, 1)
struct D2EditBoxImplStrc
{
    void** pVtable;                 // +0x00 (0x6FB84748)
    int32_t bVisible;               // +0x04
    int32_t nX;                     // +0x08
    int32_t nY;                     // +0x0C
    int32_t nWidth;                 // +0x10
    int32_t nHeight;                // +0x14
    int32_t bDrawBox;               // +0x18
    int32_t nFont;                  // +0x1C
    int32_t bNumericOnly;           // +0x20
    uint8_t nTextColor;             // +0x24
    uint8_t nBoxColor;              // +0x25
    int32_t nTransTbl;              // +0x26
    Unicode* wszText;               // +0x28
    Unicode* wszTemp;               // +0x2C
    int32_t nBufferSize;            // +0x30
    int32_t nTextLength;            // +0x34
    int32_t nMaxLength;             // +0x38
    int32_t nMaxWidth;              // +0x3C
    int32_t nMaxHeight;             // +0x40
    int32_t bHasFocus;              // +0x44
    int32_t nCursorPos;             // +0x48
    int32_t bBlinkState;            // +0x4C
    uint16_t wBlinkTick;            // +0x50
    D2CellFileStrc* pSliderCel;     // +0x52
    BOOL (__stdcall *pfnCharFilter)(char ch); // +0x56
};

struct D2EditBoxProxyStrc
{
    void** pVtable;                 // +0x00 (0x6FB846F8)
    int32_t bEnabled;               // +0x04
    D2EditBoxImplStrc* pImpl;       // +0x08
};
#pragma pack(pop)

// 1.12a: 0x6FB34180
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
void __fastcall sub_6FB34180(D2EditBoxProxyStrc* pThis)
{
    // Proxy vtable[0]: forwards Draw() to pThis->pImpl->vtable[0].
}

// 1.12a: 0x6FB34190
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB34190(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[1]: forwards OnLButtonDown(pMsg) to pThis->pImpl->vtable[1].
    return 0;
}

// 1.12a: 0x6FB341A0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB341A0(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[2]: forwards OnLButtonUp(pMsg) to pThis->pImpl->vtable[2].
    return 0;
}

// 1.12a: 0x6FB341B0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB341B0(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[3]: forwards OnMouseMove(pMsg) to pThis->pImpl->vtable[3].
    return 0;
}

// 1.12a: 0x6FB341C0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB341C0(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[4]: forwards OnKeyDown(pMsg) to pThis->pImpl->vtable[4].
    return 0;
}

// 1.12a: 0x6FB341D0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB341D0(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[5]: forwards OnKeyUp(pMsg) to pThis->pImpl->vtable[5].
    return 0;
}

// 1.12a: 0x6FB341E0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB341E0(D2EditBoxProxyStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Proxy vtable[6]: forwards OnChar(pMsg) to pThis->pImpl->vtable[6].
    return 0;
}

// 1.12a: 0x6FB341F0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
void __fastcall sub_6FB341F0(D2EditBoxProxyStrc* pThis)
{
    // Proxy vtable[7]: forwards MoveCursorLeft() to pThis->pImpl->vtable[7].
}

// 1.12a: 0x6FB34200
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
void __fastcall sub_6FB34200(D2EditBoxProxyStrc* pThis)
{
    // Proxy vtable[8]: forwards MoveCursorRight() to pThis->pImpl->vtable[8].
}

// 1.12a: 0x6FB34210
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, char* szOut)
char* __fastcall sub_6FB34210(D2EditBoxProxyStrc* pThis, void* _edx, char* szOut)
{
    // Proxy vtable[9]: forwards GetText(szOut) to pThis->pImpl->vtable[9].
    return szOut;
}

// 1.12a: 0x6FB34220
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
int32_t __fastcall sub_6FB34220(D2EditBoxProxyStrc* pThis)
{
    // Proxy vtable[10]: forwards GetIntValue() to pThis->pImpl->vtable[10].
    return 0;
}

// 1.12a: 0x6FB34230
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, BOOL bReadOnly)
void __fastcall sub_6FB34230(D2EditBoxProxyStrc* pThis, void* _edx, BOOL bReadOnly)
{
    // Proxy vtable[11]: forwards SetReadOnly(bReadOnly) to pThis->pImpl->vtable[11].
}

// 1.12a: 0x6FB34240
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, int32_t nMaxWidth)
void __fastcall sub_6FB34240(D2EditBoxProxyStrc* pThis, void* _edx, int32_t nMaxWidth)
{
    // Proxy vtable[12]: forwards SetMaxWidth(nMaxWidth) to pThis->pImpl->vtable[12].
}

// 1.12a: 0x6FB34250
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, int32_t nMaxHeight)
void __fastcall sub_6FB34250(D2EditBoxProxyStrc* pThis, void* _edx, int32_t nMaxHeight)
{
    // Proxy vtable[13]: forwards SetMaxHeight(nMaxHeight) to pThis->pImpl->vtable[13].
}

// 1.12a: 0x6FB34260
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, const char* szText)
void __fastcall sub_6FB34260(D2EditBoxProxyStrc* pThis, void* _edx, const char* szText)
{
    // Proxy vtable[14]: forwards SetText(szText) to pThis->pImpl->vtable[14].
}

// 1.12a: 0x6FB34270
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, int32_t nValue)
void __fastcall sub_6FB34270(D2EditBoxProxyStrc* pThis, void* _edx, int32_t nValue)
{
    // Proxy vtable[15]: forwards SetIntValue(nValue) to pThis->pImpl->vtable[15].
}

// 1.12a: 0x6FB34280
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, BOOL bNumericOnly)
void __fastcall sub_6FB34280(D2EditBoxProxyStrc* pThis, void* _edx, BOOL bNumericOnly)
{
    // Proxy vtable[16]: forwards SetNumericOnly(bNumericOnly) to pThis->pImpl->vtable[16].
}

// 1.12a: 0x6FB34290
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
void __fastcall sub_6FB34290(D2EditBoxProxyStrc* pThis)
{
    // Base proxy vtable destructor stub (sets vtable to 0x6FB80918).
    pThis->pVtable = (void**)0x6FB80918;
}

// 1.12a: 0x6FB342A0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB342A0(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Impl vtable[4]/[5]: default key down/up handler (returns 0).
    return 0;
}

// 1.12a: 0x6FB342B0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
void __fastcall sub_6FB342B0(D2EditBoxImplStrc* pThis)
{
    // Impl vtable[7]: MoveCursorLeft() - decrements nCursorPos if > 0.
    if (pThis->nCursorPos > 0)
        pThis->nCursorPos--;
}

// 1.12a: 0x6FB342C0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
void __fastcall sub_6FB342C0(D2EditBoxImplStrc* pThis)
{
    // Impl vtable[8]: MoveCursorRight() - increments nCursorPos if < nTextLength.
    if (pThis->nCursorPos < pThis->nTextLength)
        pThis->nCursorPos++;
}

// 1.12a: 0x6FB342D0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, BOOL bReadOnly)
void __fastcall sub_6FB342D0(D2EditBoxImplStrc* pThis, void* _edx, BOOL bReadOnly)
{
    // Impl vtable[11]: SetReadOnly(bReadOnly) - sets bHasFocus = !bReadOnly.
    pThis->bHasFocus = (bReadOnly == 0);
}

// 1.12a: 0x6FB342E0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nMaxWidth)
void __fastcall sub_6FB342E0(D2EditBoxImplStrc* pThis, void* _edx, int32_t nMaxWidth)
{
    // Impl vtable[12]: SetMaxWidth(nMaxWidth).
    if (nMaxWidth >= 0)
        pThis->nMaxWidth = nMaxWidth;
}

// 1.12a: 0x6FB34300
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, BOOL (__stdcall *pfnFilter)(char))
void __fastcall sub_6FB34300(D2EditBoxImplStrc* pThis, void* _edx, BOOL (__stdcall *pfnFilter)(char))
{
    // Impl vtable[17]: SetCharFilter(pfnFilter).
    pThis->pfnCharFilter = pfnFilter;
}

// 1.12a: 0x6FB34310
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, uint8_t bFlag)
D2EditBoxImplStrc* __fastcall sub_6FB34310(D2EditBoxImplStrc* pThis, void* _edx, uint8_t bFlag)
{
    // Base impl vtable scalar deleting destructor stub.
    pThis->pVtable = (void**)0x6FB80918;
    return pThis;
}

// 1.12a: 0x6FB34330
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
void __fastcall sub_6FB34330(D2EditBoxImplStrc* pThis)
{
    // Base impl destructor stub.
    pThis->pVtable = (void**)0x6FB80918;
}

// 1.12a: 0x6FB34340
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB34340(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Impl vtable[6]: OnChar(pMsg) - handles Backspace, Delete, Enter, Escape, and character insertion.
    return 1;
}

// 1.12a: 0x6FB34660
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB34660(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Impl vtable[1]: OnLButtonDown(pMsg) - sets bHasFocus if click is inside editbox bounds.
    return pThis->bHasFocus;
}

// 1.12a: 0x6FB346B0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB346B0(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Impl vtable[2]: OnLButtonUp(pMsg).
    return 0;
}

// 1.12a: 0x6FB346F0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB346F0(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Impl vtable[3]: OnMouseMove(pMsg).
    return 0;
}

// 1.12a: 0x6FB34710
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, char* szOut)
char* __fastcall sub_6FB34710(D2EditBoxImplStrc* pThis, void* _edx, char* szOut)
{
    // Impl vtable[9]: GetText(szOut) - converts wszText to ASCII via Unicode::unicode2Win.
    return szOut;
}

// 1.12a: 0x6FB34730
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
int32_t __fastcall sub_6FB34730(D2EditBoxImplStrc* pThis)
{
    // Impl vtable[10]: GetIntValue() - parses wszText as integer via atol.
    return 0;
}

// 1.12a: 0x6FB34760
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nMaxHeight)
void __fastcall sub_6FB34760(D2EditBoxImplStrc* pThis, void* _edx, int32_t nMaxHeight)
{
    // Impl vtable[13]: SetMaxHeight(nMaxHeight).
}

// 1.12a: 0x6FB34780
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, const char* szText)
void __fastcall sub_6FB34780(D2EditBoxImplStrc* pThis, void* _edx, const char* szText)
{
    // Impl vtable[14]: SetText(szText) - feeds each character in szText through OnChar (vtable[6]).
}

// 1.12a: 0x6FB347B0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, BOOL bNumericOnly)
void __fastcall sub_6FB347B0(D2EditBoxImplStrc* pThis, void* _edx, BOOL bNumericOnly)
{
    // Impl vtable[16]: SetNumericOnly(bNumericOnly).
    pThis->bNumericOnly = bNumericOnly;
}

// 1.12a: 0x6FB347C0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, BOOL (__stdcall *pfnFilter)(char))
void __fastcall sub_6FB347C0(D2EditBoxProxyStrc* pThis, void* _edx, BOOL (__stdcall *pfnFilter)(char))
{
    // Proxy vtable[17]: forwards SetCharFilter(pfnFilter) to pThis->pImpl->vtable[17].
}

// 1.12a: 0x6FB347D0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this)
void __fastcall sub_6FB347D0(D2EditBoxProxyStrc* pThis)
{
    // CEditBoxProxy destructor: destroys pImpl (vtable[18]) and resets vtable to 0x6FB80918.
}

// 1.12a: 0x6FB347F0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nValue)
void __fastcall sub_6FB347F0(D2EditBoxImplStrc* pThis, void* _edx, int32_t nValue)
{
    // Impl vtable[15]: SetIntValue(nValue) - formats nValue with "%d" and calls SetText (vtable[14]).
}

// 1.12a: 0x6FB34810
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, uint8_t bFlags)
D2EditBoxProxyStrc* __fastcall sub_6FB34810(D2EditBoxProxyStrc* pThis, void* _edx, uint8_t bFlags)
{
    // Proxy vtable[18]: scalar deleting destructor for D2EditBoxProxyStrc.
    sub_6FB347D0(pThis);
    return pThis;
}

// 1.12a: 0x6FB34830
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
void __fastcall sub_6FB34830(D2EditBoxImplStrc* pThis)
{
    // Impl vtable[0]: Draw() - draws optional box background, blinking '_' cursor, and single/multi-line text.
}

// 1.12a: 0x6FB34B10
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, uint8_t bFlags)
D2EditBoxImplStrc* __fastcall sub_6FB34B10(D2EditBoxImplStrc* pThis, void* _edx, uint8_t bFlags)
{
    // Impl vtable[18]: scalar deleting destructor for D2EditBoxImplStrc.
    return pThis;
}

// 1.12a: 0x6FB34B30
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
void __fastcall sub_6FB34B30(D2EditBoxImplStrc* pThis)
{
    // CEditBoxImpl destructor: frees wszText, wszTemp, and pSliderCel (textslid).
}

// 1.12a: 0x6FB34BE0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nX, int32_t nY, int32_t nW, int32_t nH, BOOL bDrawBox, int32_t nFont, BOOL bNumeric, uint8_t nTextCol, uint8_t nBoxCol, int32_t nTransTbl, int32_t nMaxLen)
D2EditBoxImplStrc* __fastcall sub_6FB34BE0(
    D2EditBoxImplStrc* pThis,
    void* _edx,
    int32_t nX,
    int32_t nY,
    int32_t nWidth,
    int32_t nHeight,
    BOOL bDrawBox,
    int32_t nFont,
    BOOL bNumericOnly,
    uint8_t nTextColor,
    uint8_t nBoxColor,
    int32_t nTransTbl,
    int32_t nMaxLength)
{
    // CEditBoxImpl constructor: allocates Unicode buffers and loads DATA\GLOBAL\UI\MENU\textslid.
    return pThis;
}

// 1.12a: 0x6FB34DE0
// Calling convention: __thiscall (ecx = D2EditBoxProxyStrc* this, int32_t nX, int32_t nY, int32_t nW, int32_t nH, BOOL bDrawBox, int32_t nFont, BOOL bNumeric, uint8_t nTextCol, uint8_t nBoxCol, int32_t nTransTbl, int32_t nMaxLen)
D2EditBoxProxyStrc* __fastcall sub_6FB34DE0(
    D2EditBoxProxyStrc* pThis,
    void* _edx,
    int32_t nX,
    int32_t nY,
    int32_t nWidth,
    int32_t nHeight,
    BOOL bDrawBox,
    int32_t nFont,
    BOOL bNumericOnly,
    uint8_t nTextColor,
    uint8_t nBoxColor,
    int32_t nTransTbl,
    int32_t nMaxLength)
{
    // CEditBoxProxy constructor: allocates a D2EditBoxImplStrc (sizeof = 0x5A) and constructs it via sub_6FB34BE0.
    return pThis;
}

// ============================================================================
// PART 3: UI_chat.asm (0x6FB34E90..0x6FB373F0 - 30 functions)
// In-game Chat Input Box, Chat History Ring Buffer, Slash Commands (/msg, /w,
// /fps, /players, /nopickup), IME Composition & Candidate List, and Realm Chat.
// ============================================================================

// 1.12a: 0x6FB34E90
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB34E90()
{
    // Clears the current chat input buffer (0x6FBCFC40) and resets length (0x6FB8FCC8 = 0).
    *(Unicode*)0x6FBCFC40 = 0;
    *(int32_t*)0x6FB8FCC8 = 0;
}

// 1.12a: 0x6FB34EB0
// Calling convention: __fastcall / register (eax = Unicode* wszTarget, edi = Unicode* wszSource)
void __fastcall sub_6FB34EB0(Unicode* wszTarget, Unicode* wszSource)
{
    // Splits a "<CharName>*<AccountName>" whisper string into account name (or char name if no '*').
}

// 1.12a: 0x6FB34F40
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB34F40()
{
    // Clears the 8-entry chat history ring buffer (0x6FBCFE48, stride 0x202) and resets indices.
}

// 1.12a: 0x6FB34FB0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB34FB0()
{
    // Pushes the current chat input buffer (0x6FBCFC40) into the 8-entry chat history ring buffer (0x6FBCFE48).
}

// 1.12a: 0x6FB35060
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB35060()
{
    // Returns TRUE if the current chat input buffer (0x6FBCFC40) begins with '/' (0x2F).
    return (*(uint16_t*)0x6FBCFC40 == 0x2F);
}

// 1.12a: 0x6FB35080
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB35080()
{
    // Parses "/w ", "/whisper ", "/m ", or "/msg " in 0x6FBCFC40 and extracts the target recipient into 0x6FBCFA28.
    return FALSE;
}

// 1.12a: 0x6FB35220
// Calling convention: __stdcall (const char* szRecipient)
void __stdcall sub_6FB35220(const char* szRecipient)
{
    // Sets the last whisper sender name (0x6FBD0E60) when a whisper is received.
}

// 1.12a: 0x6FB35260
// Calling convention: __stdcall (0 args)
Unicode __stdcall sub_6FB35260()
{
    // Updates the 250ms chat cursor blink timer (0x6FBD0E80) and returns '_' or 0.
    return 0;
}

// 1.12a: 0x6FB352B0
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB352B0()
{
    // Appends the blinking cursor character to 0x6FBCFC40 before drawing; returns TRUE if appended.
    return FALSE;
}

// 1.12a: 0x6FB352F0
// Calling convention: __fastcall / register (eax = const Unicode* wszStr, ecx = const Unicode* wszPrefix)
BOOL __fastcall sub_6FB352F0(const Unicode* wszStr, const Unicode* wszPrefix)
{
    // Case-insensitive Unicode prefix match helper for slash commands.
    return FALSE;
}

// 1.12a: 0x6FB35350
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB35350()
{
    // Returns 0x6FBCFA10 (whether the chat input box is currently active).
    return *(int32_t*)0x6FBCFA10;
}

// 1.12a: 0x6FB35360
// Calling convention: __fastcall (ecx = int32_t bActive)
void __fastcall sub_6FB35360(int32_t bActive)
{
    // Sets 0x6FBCFA10 = bActive and clears chat input buffer if deactivated.
    *(int32_t*)0x6FBCFA10 = bActive;
    if (!bActive)
    {
        *(Unicode*)0x6FBCFC40 = 0;
        *(int32_t*)0x6FB8FCC8 = 0;
    }
}

// 1.12a: 0x6FB35380
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB35380(D2MsgStrc* pMsg)
{
    // Consumes IME messages (0x0283, 0x0284, 0x0285) by marking pMsg handled and breaking chain.
}

// 1.12a: 0x6FB353A0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB353A0()
{
    // Detects active keyboard layout / IME status (GetKeyboardLayout, ImmIsIME) and sets 0x6FBCFA08.
}

// 1.12a: 0x6FB353F0
// Calling convention: __stdcall (HIMC hIMC)
void __stdcall sub_6FB353F0(HIMC hIMC)
{
    // Queries IME candidate list (ImmGetCandidateListA) into 0x6FBCFA18.
}

// 1.12a: 0x6FB35480
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB35480(D2MsgStrc* pMsg)
{
    // WM_IME_ENDCOMPOSITION handler: frees IME composition buffers (0x6FBCFA14, 0x6FBCFA1C).
}

// 1.12a: 0x6FB354D0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB354D0(D2MsgStrc* pMsg)
{
    // WM_IME_STARTCOMPOSITION handler: allocates IME composition buffers (0x6FBCFA14, 0x6FBCFA1C).
}

// 1.12a: 0x6FB35540
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB35540(D2MsgStrc* pMsg)
{
    // VK_UP / VK_DOWN handler while chat input is open: navigates chat history ring buffer (0x6FBCFE48).
}

// 1.12a: 0x6FB355A0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB355A0()
{
    // Renders the IME candidate list popup box when 0x6FBCFA20 != 0.
}

// 1.12a: 0x6FB35720
// Calling convention: __fastcall / register (eax = int32_t nX, ecx = int32_t nY)
void __fastcall sub_6FB35720(int32_t nX, int32_t nY)
{
    // Renders the active IME composition string and cursor at (nX, nY).
}

// 1.12a: 0x6FB357E0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB357E0()
{
    // Unregisters chat input message handlers (0x6FB8FCDC) and IME handlers (0x6FB8FD10).
}

// 1.12a: 0x6FB358F0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB358F0()
{
    // Opens the chat input box and registers chat & IME message handlers (0x6FB8FCDC, 0x6FB8FD10).
}

// 1.12a: 0x6FB35BA0
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB35BA0(D2MsgStrc* pMsg)
{
    // WM_IME_NOTIFY handler: updates IME status, conversion mode, and candidate list window state.
}

// 1.12a: 0x6FB35DF0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB35DF0()
{
    // Polls the Battle.net Realm chat interface ([0x6FBCB908]->pChat) and dispatches realm whispers/system messages.
}

// 1.12a: 0x6FB36110
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB36110(D2MsgStrc* pMsg)
{
    // WM_IME_COMPOSITION handler: processes GCS_RESULTSTR and GCS_COMPSTR via ImmGetCompositionStringA.
}

// 1.12a: 0x6FB36340
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB36340()
{
    // Pre-validates "/msg", "/m", "/whisper", "/w", "/help", "/?" commands in the chat input buffer.
    return FALSE;
}

// 1.12a: 0x6FB36540
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB36540()
{
    // Checks if chat input begins with a realm '/' command and sends it via Battle.net chat callback if connected.
    return FALSE;
}

// 1.12a: 0x6FB36630
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB36630()
{
    // Processes Enter in the chat box: handles local commands (/fps, /framerate, /players, /nopickup,
    // soundchaosdebug) or sends C2S 0x14 (overhead !) / C2S 0x15 (chat/whisper).
}

// 1.12a: 0x6FB36A80
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB36A80(D2MsgStrc* pMsg)
{
    // WM_LBUTTONDOWN handler while chat input is open: closes chat input box and unregisters handlers.
}

// 1.12a: 0x6FB36BC0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB36BC0()
{
    // Renders the in-game chat input prompt ("Say:", "Whisper:", "Overhead:"), wrapped input text, and IME UI.
}

// 1.12a: 0x6FB37170 (entry 0x6FB36F80 in 0x6FB8FCDC)
// Calling convention: __stdcall (D2MsgStrc* pMsg)
void __stdcall sub_6FB37170(D2MsgStrc* pMsg)
{
    // WM_CHAR handler for the in-game chat input box: handles Enter, Escape, Backspace, Tab/whisper shortcuts,
    // and printable / DBCS character insertion.
}

// ============================================================================
// PART 4: Additional Helper & Sub-functions across UI_text, UI_editbox, UI_chat
// ============================================================================

// 1.12a: 0x6FB1F3E0
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB1F3E0()
{
    // Opens the NPC trade/dialog UI panel (0x18) via sub_6FB3EF00.
    return TRUE;
}

// 1.12a: 0x6FB1F400
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB1F400()
{
    // Returns TRUE if an NPC speech box (0x6FBB5490) is currently allocated.
    return (*(void**)0x6FBB5490 != NULL);
}

// 1.12a: 0x6FB1F410
// Calling convention: __fastcall / register (eax = uint8_t* pPacket)
void __fastcall sub_6FB1F410(uint8_t* pPacket)
{
    // Stores 3 DWORD parameters from packet (+3, +7, +11) into 0x6FBB537C..0x6FBB5384 when word[+1] == 4.
    if (*(uint16_t*)(pPacket + 1) == 4)
    {
        *(uint32_t*)0x6FBB537C = *(uint32_t*)(pPacket + 3);
        *(uint32_t*)0x6FBB5380 = *(uint32_t*)(pPacket + 7);
        *(uint32_t*)0x6FBB5384 = *(uint32_t*)(pPacket + 11);
        *(int32_t*)0x6FBB553C = 0;
    }
}

// 1.12a: 0x6FB1F440
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F440()
{
    // No-op stub.
}

// 1.12a: 0x6FB1F450
// Calling convention: __fastcall (edx = uint16_t wKeyId)
const Unicode* __fastcall sub_6FB1F450(int32_t _ecx, uint16_t wKeyId)
{
    // Looks up the localized string for a virtual key code in 0x6FB92968.
    return NULL;
}

// 1.12a: 0x6FB1F4A0
// Calling convention: __stdcall (0 args)
void* __stdcall sub_6FB1F4A0()
{
    // Returns the active NPC text header pointer (0x6FBB5538).
    return *(void**)0x6FBB5538;
}

// 1.12a: 0x6FB1F4B0
// Calling convention: __stdcall (0 args)
int32_t __stdcall sub_6FB1F4B0()
{
    // Returns the 'Text Display Beta' registry setting value (0x6FBB54DE).
    return *(int32_t*)0x6FBB54DE;
}

// 1.12a: 0x6FB1F4C0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F4C0()
{
    // Computes and stores the NPC speech box viewport clip rectangle (0x6FBB5544..0x6FBB5550) and sets 0x6FBCC120 = 1.
}

// 1.12a: 0x6FB1F520
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F520()
{
    // No-op stub.
}

// 1.12a: 0x6FB1F530
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F530()
{
    // No-op stub.
}

// 1.12a: 0x6FB1F540
// Calling convention: __fastcall / register (eax = void* pfnCallback, ecx = int32_t nArg)
void __fastcall sub_6FB1F540(void* pfnCallback, int32_t nArg)
{
    // Registers a post-speech callback (0x6FBCC0F4) and context argument (0x6FBCC0F8).
    *(void**)0x6FBCC0F4 = pfnCallback;
    *(int32_t*)0x6FBCC0F8 = nArg;
}

// 1.12a: 0x6FB1F550
// Calling convention: __stdcall (0 args)
uint32_t __stdcall sub_6FB1F550()
{
    // Returns GetTickCount() + 10000 (10-second duration for chat messages).
    return GetTickCount() + 10000;
}

// 1.12a: 0x6FB1F560
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F560()
{
    // Re-populates the editbox/log control (0x6FBB549C) from the message history list (0x6FBB54D8).
}

// 1.12a: 0x6FB1F8B0
// Calling convention: __fastcall / register (eax = D2NpcSpeechBoxStrc* pBox)
BOOL __fastcall sub_6FB1F8B0(D2NpcSpeechBoxStrc* pBox)
{
    // Returns TRUE if pBox is non-null and its state (+0x16) is 0 or 1.
    return (pBox != NULL && (pBox->nState == 0 || pBox->nState == 1));
}

// 1.12a: 0x6FB1F8D0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F8D0()
{
    // Clears the NPC speech click-target valid byte (0x6FBB5522 = 0).
    *(uint8_t*)0x6FBB5522 = 0;
}

// 1.12a: 0x6FB1F900
// Calling convention: __fastcall (ecx = uint32_t nVal)
uint32_t __fastcall sub_6FB1F900(uint32_t nVal)
{
    // Computes 0x70 - (nVal >> 10).
    return 0x70 - (nVal >> 10);
}

// 1.12a: 0x6FB1F930
// Calling convention: __fastcall / register (edi = void** ppLinesNode)
void __fastcall sub_6FB1F930(void** ppLinesNode)
{
    // Frees up to 6 line strings in a chat message node and frees the node itself.
}

// 1.12a: 0x6FB1F970
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB1F970()
{
    // Zeroes chat/text state (0x6FBB5498..0x6FBB5536) and loads 'Text Display Beta' from registry.
}

// 1.12a: 0x6FB1F9C0
// Calling convention: __fastcall / register (eax = void* pBox)
void __fastcall sub_6FB1F9C0(void* pBox)
{
    // Resets current scroll offset (+0x0C) to initial height (+0x14).
}

// 1.12a: 0x6FB1F9D0
// Calling convention: __fastcall / register (eax = void* pLineList)
void __fastcall sub_6FB1F9D0(void* pLineList)
{
    // Frees a linked list of wrapped Unicode text lines.
}

// 1.12a: 0x6FB1FA10
// Calling convention: __userpurge (esi = const Unicode** ppSrc, ebx = int32_t nMaxChars, arg0 = Unicode* pDst)
void __stdcall sub_6FB1FA10(Unicode* pDst)
{
    // Extracts the next line up to '\n' or nMaxChars from *ppSrc into pDst.
}

// 1.12a: 0x6FB1FBC0
// Calling convention: __stdcall (0 args)
D2UnitStrc* __stdcall sub_6FB1FBC0()
{
    // Finds and returns the client unit for the currently speaking or interacting NPC.
    return NULL;
}

// 1.12a: 0x6FB200D0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB200D0()
{
    // Tail of message expiration loop in screen chat rendering: unlinks and frees expired message node.
}

// 1.12a: 0x6FB20120
// Calling convention: __fastcall / register (eax = int32_t nType)
void __fastcall sub_6FB20120(int32_t nType)
{
    // Enforces maximum message count (128) in 0x6FBB54D8 by freeing the oldest tail node.
}

// 1.12a: 0x6FB201A0
// Calling convention: __fastcall / register (eax = int32_t bRegister)
void __fastcall sub_6FB201A0(int32_t bRegister)
{
    // Registers (bRegister == 1) or unregisters (bRegister == 0) the NPC speech message handler table (0x6FB928F0).
}

// 1.12a: 0x6FB20210
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit)
void __fastcall sub_6FB20210(D2UnitStrc* pUnit)
{
    // Records the clicked unit GUID into 0x6FBB5523 and sets 0x6FBB5522 = 1.
}

// 1.12a: 0x6FB20230
// Calling convention: __stdcall (void* pSpeechBox, int32_t nX, int32_t nY)
void __stdcall sub_6FB20230(void* pSpeechBox, int32_t nX, int32_t nY)
{
    // Draws a static multi-line text box at (nX, nY) with framed border.
}

// 1.12a: 0x6FB20660
// Calling convention: __stdcall (void* pMsgList)
void __stdcall sub_6FB20660(void* pMsgList)
{
    // Frees all message nodes in a chat message list (0x6FBB54CC or 0x6FBB54D8).
}

// 1.12a: 0x6FB20710
// Calling convention: __fastcall / register (edi = void* pSpeechBox)
void __fastcall sub_6FB20710(void* pSpeechBox)
{
    // Computes the maximum line width and total pixel height for a wrapped speech box.
}

// 1.12a: 0x6FB20A20
// Calling convention: __fastcall / register (eax = D2UnitStrc* pUnit, esi = int32_t nStringId)
void __fastcall sub_6FB20A20(D2UnitStrc* pUnit, int32_t nStringId)
{
    // Adjusts NPC speech box dimensions for CJK languages (Korean, Chinese, Japanese).
}

// 1.12a: 0x6FB20B50
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20B50()
{
    // Frees both the screen chat list (0x6FBB54CC) and the message log history list (0x6FBB54D8).
    sub_6FB20660((void*)0x6FBB54CC);
    sub_6FB20660((void*)0x6FBB54D8);
}

// 1.12a: 0x6FB20B70
// Calling convention: __fastcall / register (edi = D2HoverTextStrc* pHoverText)
void __fastcall sub_6FB20B70(D2HoverTextStrc* pHoverText)
{
    // Frees the client wrapped-line structure attached to pHoverText and frees pHoverText via D2Common#10375.
}

// 1.12a: 0x6FB20BB0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20BB0()
{
    // Stops any active NPC speech audio stream (0x6FBCC0FC) and clears 0x6FBB5533.
}

// 1.12a: 0x6FB20CB0
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB20CB0()
{
    // Frees active NPC text header (0x6FBB5538), NPC speech box (0x6FBB5490), and barkscroll text (0x6FBB5488).
}

// 1.12a: 0x6FB21210
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB21210()
{
    // Closes active NPC speech (sub_6FB21170), clears 0x6FBB54E2, and frees 0x6FBB5538 via TEXT_FreeTextHeader.
}

// 1.12a: 0x6FB21240
// Calling convention: __stdcall (uint16_t wStringId)
void __stdcall sub_6FB21240(uint16_t wStringId)
{
    // Frees previous barkscroll text (0x6FBB5488) and begins formatting wStringId for barkscroll display.
}

// 1.12a: 0x6FB21290
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB21290()
{
    // Completes allocation and word-wrapping of barkscroll text (0x6FBB5488).
}

// 1.12a: 0x6FB21500
// Calling convention: __stdcall (const Unicode* wszText, uint8_t nColor, int32_t bSystemMsg)
void __stdcall sub_6FB21500(const Unicode* wszText, uint8_t nColor, int32_t bSystemMsg)
{
    // Word-wraps wszText into a chat message node (sizeof = 0x26) and inserts it into the active message list.
}

// 1.12a: 0x6FB21D70
// Calling convention: __fastcall / register (eax = uint8_t* pPacket)
void __fastcall sub_6FB21D70(uint8_t* pPacket)
{
    // Handles S2C NPC interaction / speech trigger packet: allocates text header and starts speech.
}

// 1.12a: 0x6FB22EC0
// Calling convention: __stdcall (0 args)
BOOL __stdcall sub_6FB22EC0()
{
    // Continuation of sub_6FB22E40: closes barkscroll or advances NPC speech and invokes post-speech callback.
    return TRUE;
}

// 1.12a: 0x6FB342F0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nMaxHeight)
void __fastcall sub_6FB342F0(D2EditBoxImplStrc* pThis, void* _edx, int32_t nMaxHeight)
{
    // Sets field +0x18 on D2EditBoxImplStrc.
    pThis->bDrawBox = nMaxHeight;
}

// 1.12a: 0x6FB34320
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nArg)
int32_t __fastcall sub_6FB34320(D2EditBoxImplStrc* pThis, void* _edx, int32_t nArg)
{
    // Returns 0 (stub virtual method).
    return 0;
}

// 1.12a: 0x6FB34680
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, D2MsgStrc* pMsg)
int32_t __fastcall sub_6FB34680(D2EditBoxImplStrc* pThis, void* _edx, D2MsgStrc* pMsg)
{
    // Returns 0 (OnLButtonUp / OnMouseMove stub).
    return 0;
}

// 1.12a: 0x6FB34690
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, int32_t nArg)
int32_t __fastcall sub_6FB34690(D2EditBoxImplStrc* pThis, void* _edx, int32_t nArg)
{
    // Sets *(int32_t*)((uint8_t*)pThis + 0x3E) = 1 and returns 0.
    return 0;
}

// 1.12a: 0x6FB346A0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
int32_t __fastcall sub_6FB346A0(D2EditBoxImplStrc* pThis)
{
    // Returns 0.
    return 0;
}

// 1.12a: 0x6FB346D0
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this)
int32_t __fastcall sub_6FB346D0(D2EditBoxImplStrc* pThis)
{
    // Converts wszText to ASCII and parses it as integer via atol.
    return 0;
}

// 1.12a: 0x6FB34720
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, const char* szText)
void __fastcall sub_6FB34720(D2EditBoxImplStrc* pThis, void* _edx, const char* szText)
{
    // Clears current editbox text and feeds each character of szText into OnChar.
}

// 1.12a: 0x6FB34750
// Calling convention: __thiscall (ecx = D2EditBoxImplStrc* this, BOOL bNumericOnly)
void __fastcall sub_6FB34750(D2EditBoxImplStrc* pThis, void* _edx, BOOL bNumericOnly)
{
    // Sets bNumericOnly (+0x20) on D2EditBoxImplStrc.
    pThis->bNumericOnly = bNumericOnly;
}

// 1.12a: 0x6FB34EF0
// Calling convention: __fastcall / register (esi = const char* szName)
const char* __fastcall sub_6FB34EF0(const char* szName)
{
    // Returns pointer to the account name portion after '*' in szName, or szName if no '*' is present.
    const char* pStar = strchr(szName, '*');
    if (pStar && pStar[1] != '\0')
        return pStar + 1;
    return szName;
}

// 1.12a: 0x6FB34F10
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB34F10()
{
    // Resets chat history ring buffer head/tail indices (0x6FB8FCD4 = 7, 0x6FB8FCD8 = 7, 0x6FBCBF44 = 0).
    *(int32_t*)0x6FB8FCD4 = 7;
    *(int32_t*)0x6FB8FCD8 = 7;
    *(int32_t*)0x6FBB2BF0 = 0;
    *(uint16_t*)0x6FBCBF44 = 0;
    *(int32_t*)0x6FBCBF48 = 0;
}

// 1.12a: 0x6FB34F70
// Calling convention: __stdcall (Unicode** ppText, int32_t* pMode, char* szRecipient)
int32_t __stdcall sub_6FB34F70(Unicode** ppText, int32_t* pMode, char* szRecipient)
{
    // Matches chat prefix table (0x6FB8FD64: '/', '!', '@') and extracts whisper target if applicable.
    return 0;
}

// 1.12a: 0x6FB35130
// Calling convention: __fastcall (ecx = int32_t nDirection)
void __fastcall sub_6FB35130(int32_t nDirection)
{
    // Steps the chat history index (0x6FBB2BF0) forward/backward within the 8-slot ring buffer.
}

// 1.12a: 0x6FB35180
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB35180()
{
    // Stores the current chat input buffer (0x6FBCFC40) into the next slot of the 8-slot history ring buffer (0x6FBCFE48).
}

// 1.12a: 0x6FB35200
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB35200()
{
    // Copies the selected chat history slot (0x6FBB2BF0) from 0x6FBCFE48 into the active input buffer (0x6FBCFC40).
}

// 1.12a: 0x6FB35240
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB35240()
{
    // No-op stub.
}

// 1.12a: 0x6FB35250
// Calling convention: __stdcall (0 args)
void __stdcall sub_6FB35250()
{
    // No-op stub.
}
