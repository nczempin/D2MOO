#pragma once

#include <D2BasicTypes.h>
#include <D2Config.h>
#include <D2Dll.h>
#include <Windows.h>
#include <cstddef>
#include <cstdint>

// 1.12a Image Base: 0x6FAB0000
constexpr int D2ClientImageBase = 0x6FAB0000;

#pragma pack(push, 1)

enum D2ClientModuleId : int32_t
{
	D2CLIENT_MODULE_NONE        = 0,
	D2CLIENT_MODULE_CLIENT      = 1,
	D2CLIENT_MODULE_SERVER      = 2,
	D2CLIENT_MODULE_MULTIPLAYER = 3,
	D2CLIENT_MODULE_LAUNCHER    = 4,
	D2CLIENT_MODULE_EXPAND      = 5,
};

enum D2ClientStateId : int32_t
{
	D2CLIENT_STATE_NONE         = 0,
	D2CLIENT_STATE_EXIT         = 1,
	D2CLIENT_STATE_GAME_LOOP    = 2,
	D2CLIENT_STATE_INIT_SESSION = 3,
	D2CLIENT_STATE_PREPARE_NET  = 4,
	D2CLIENT_STATE_START_NET    = 5,
	D2CLIENT_STATE_LEAVE_GAME   = 6,
	D2CLIENT_STATE_COUNT        = 7,
};

struct D2ClientInterfaceStrc
{
	int32_t(__fastcall* pfMainEntry)(D2ConfigStrc* pCfg);
};

// 1.12a 0x2E-byte (46-byte) D2GS_GAMELOGON (0x67) client join packet built at 0x6FB294E0
struct D2ClientLogonPacketStrc
{
	uint8_t nHeader;                       // 0x00 (0x67)
	char szCharName[16];                   // 0x01
	uint8_t nGameMode;                     // 0x11 (3=SinglePlayer, 1/2=TCPIP, 0=Closed)
	uint8_t nCharClass;                    // 0x12
	uint8_t nArenaTemplate;                // 0x13
	uint8_t nArenaDifficulty;              // 0x14
	char szAccountName[16];                // 0x15
	uint16_t wArena;                       // 0x25
	uint32_t dwArenaFlags;                 // 0x27
	uint8_t nUnk2B;                        // 0x2B
	uint8_t nUnk2C;                        // 0x2C
	uint8_t nLocaleId;                     // 0x2D
	uint16_t wUnk2E;                       // 0x2E (padding to 0x30 stack frame; 0x2E bytes sent)
};
static_assert(offsetof(D2ClientLogonPacketStrc, wUnk2E) == 0x2E, "Logon packet payload size must be 0x2E");

#pragma pack(pop)

// 1.12a @10001: 0x6FB4E980
extern "C" D2CLIENT_DLL_DECL int32_t __stdcall D2Client_10001(int32_t nUnused);
// 1.12a @10002: 0x6FB4E940
extern "C" D2CLIENT_DLL_DECL uint8_t __stdcall D2Client_10002(int32_t nUnused, int32_t nIndex);
// 1.12a @10003: 0x6FAC2100
extern "C" D2CLIENT_DLL_DECL void __stdcall D2Client_10003();
// 1.12a @10004: 0x6FB535D0
extern "C" D2CLIENT_DLL_DECL D2ClientInterfaceStrc* __stdcall QueryInterface();

// 1.12a: 0x6FB53600
int32_t __fastcall D2Client_MainEntry(D2ConfigStrc* pCfg);
// 1.12a: 0x6FAC24C0
int32_t __fastcall D2Client_RunGameLoop(D2ConfigStrc* pCfg);
// 1.12a: 0x6FB294E0
void __fastcall D2Client_SendJoinGamePacket(const char* szCharName);
