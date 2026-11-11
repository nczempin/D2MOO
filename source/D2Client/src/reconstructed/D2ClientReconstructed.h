#pragma once

#include <windows.h>
#include <stdint.h>

struct D2UnitStrc;
struct D2DrlgActStrc;
struct D2CellFileStrc;
struct D2GfxCellStrc;
struct D2RosterUnitStrc;
struct D2RosterPetEntryStrc;
struct D2BitBufferStrc;
struct D2StatListStrc;
struct D2InventoryStrc;
struct D2SkillStrc;

#ifndef D2_GFX_DATA_STRC_DEFINED
#define D2_GFX_DATA_STRC_DEFINED
#pragma pack(push, 1)
struct D2GfxDataStrc
{
    uint8_t fState;                         // +0x00
    union                                   // +0x01
    {
        uint8_t nComponent;
        uint8_t fItemFlags;
    };
    uint8_t unk0x02;                        // +0x02
    uint8_t unk0x03;                        // +0x03
    int32_t nUnitType;                      // +0x04
    union                                   // +0x08
    {
        int32_t nUnitIndex;
        int32_t nClassId;
    };
    int32_t nMode;                          // +0x0C
    union                                   // +0x10
    {
        int32_t nOverlay;
        int32_t nOverlayId;
    };
    union                                   // +0x14
    {
        struct
        {
            char szToken[4];                // +0x14
            union                           // +0x18
            {
                char szComponent[4];
                char szCompCode[4];
            };
            char szArmorType[4];            // +0x1C
            char szMode[4];                 // +0x20
            union                           // +0x24
            {
                char szHitClass[4];
                char szWeaponClass[4];
            };
        };
        uint32_t dwName[5];                 // +0x14..+0x27
    };
    char* pName;                            // +0x28
    union                                   // +0x2C
    {
        uint32_t fFlags;
        uint32_t dwFlags;
    };
    int32_t nMaxDirections;                 // +0x30
    union                                   // +0x34
    {
        D2GfxCellStrc* pCurrentCell;
        D2GfxCellStrc* pCell;
    };
    union                                   // +0x38
    {
        uint32_t nDirection;
        int32_t nDir;
    };
    union                                   // +0x3C
    {
        D2CellFileStrc* pCellFile;
        D2CellFileStrc* pCelFile;
    };
    uint32_t nFrame;                        // +0x40
    int32_t nMaxFrames;                     // +0x44
};
#pragma pack(pop)
static_assert(sizeof(D2GfxDataStrc) == 0x48, "1.12a D2GfxDataStrc must be 0x48 bytes");
#endif

enum D2ClientPanelId : int32_t
{
    D2PANEL_NONE         = 0x00,
    D2PANEL_INVENTORY    = 0x01,
    D2PANEL_CHAR         = 0x02,
    D2PANEL_SKILLSELECT  = 0x03,
    D2PANEL_SKILLTREE    = 0x04,
    D2PANEL_CHAT         = 0x05,
    D2PANEL_NEWSTATS     = 0x06,
    D2PANEL_NEWSKILLS    = 0x07,
    D2PANEL_NPCMENU      = 0x08,
    D2PANEL_ESCMENU      = 0x09,
    D2PANEL_AUTOMAP      = 0x0A,
    D2PANEL_CFGCONTROLS  = 0x0B,
    D2PANEL_NPCSHOP      = 0x0C,
    D2PANEL_SHOWITEMS    = 0x0D,
    D2PANEL_MODITEM      = 0x0E,
    D2PANEL_QUEST        = 0x0F,
    D2PANEL_WAYPOINT     = 0x10,
    D2PANEL_MINIPANEL    = 0x15,
    D2PANEL_PARTY        = 0x16,
    D2PANEL_TRADEPROMPT  = 0x17,
    D2PANEL_MSGLOG       = 0x18,
    D2PANEL_STASH        = 0x19,
    D2PANEL_CUBE         = 0x1A,
    D2PANEL_BELT         = 0x1F,
    D2PANEL_MERCINV      = 0x24,
    D2PANEL_COUNT        = 0x26
};

// Canonical shared D2Client globals
extern D2UnitStrc*           gpPlayerUnit;
extern D2DrlgActStrc*        gpClientAct;
extern void*                 gpClientViewport;

extern int32_t               gnScreenWidth;
extern int32_t               gnScreenHeight;
extern int32_t               gnViewportWidth;
extern int32_t               gnViewportHeight;
extern int32_t               gnViewportShiftMode;
extern int32_t               gnViewportShiftOffsetX;
extern int32_t               gnCameraScreenSkewX;
extern int32_t               gnCameraScreenSkewY;
extern int32_t               g_nPanelOffsetX;
extern int32_t               g_nPanelOffsetY;

extern int32_t               gnMouseX;
extern int32_t               gnMouseY;
extern int32_t               gnCursorState;
extern D2CellFileStrc*       gpCursorItemCellFile;

extern int32_t               gaPanelStates[38];
extern int32_t               gnPanelCount;

extern D2UnitStrc*           gapServerSideUnitHashTables[6][128];
extern D2UnitStrc*           gapClientSideUnitHashTables[6][128];

extern D2RosterUnitStrc*     gpRosterUnitList;
extern D2RosterPetEntryStrc* gpRosterPetList;

extern int32_t               gbIsExpansion;
extern uint8_t               gnClientDifficulty;
extern int32_t               gnGameType;
extern uint32_t              gdwClientGameTick;
extern uint32_t              gdwClientPing;
extern uint32_t              gdwReassignGraceTick;

extern int32_t               g_bScreenShakeActive;
extern int32_t               g_nShakeOffsetX;
extern int32_t               g_nShakeOffsetY;
extern int32_t               g_nShakeMaxAmplitude;
extern uint32_t              g_dwShakeStartTick;
extern int32_t               g_nShakeRampUpMs;
extern int32_t               g_nShakeHoldMs;
extern int32_t               g_nShakeRampDownMs;

extern int32_t               g_bInteractingWithNpc;
extern int32_t               g_nInteractingNpcGUID;
extern int32_t               g_nInteractingNpcClassId;
extern D2UnitStrc*           g_pTradeNpcUnit;
extern D2BitBufferStrc*      g_pPlayerQuestRecord;
extern D2BitBufferStrc*      g_pGameQuestRecord;

struct D2ClientItemStatDescInfoStrc
{
    int16_t  nDescPriority;
    uint8_t  nDescFunc;
    uint8_t  nDescVal;
    uint16_t wDescStrPos;
    uint16_t wDescStrNeg;
    uint16_t wDescStr2;
    uint16_t wDescGrp;
    uint8_t  nDescGrpFunc;
    uint8_t  nDescGrpVal;
    uint16_t wDescGrpStrPos;
    uint16_t wDescGrpStrNeg;
    uint16_t wDescGrpStr2;
    uint8_t  nOp;
    uint8_t  nOpParam;
    uint16_t wOpBase;
};

int32_t     __fastcall D2Client_GetPetTypeCount();
const void* __fastcall D2Client_GetPetTypeRecord(int32_t nPetType);
int32_t     __fastcall D2Client_GetItemStatCostCount();
int32_t     __fastcall D2Client_GetItemStatDescInfo(int32_t nStatId, D2ClientItemStatDescInfoStrc* pOut);
int         __fastcall D2Client_PopulateUnitComponentGfxNames(D2UnitStrc* pUnit, D2GfxDataStrc* pOutGfxData, int nComponent, const char* szWeaponClass);
int         __fastcall D2Client_PopulateNonCompositeGfxName(D2UnitStrc* pUnit, D2GfxDataStrc* pOutGfxData);
