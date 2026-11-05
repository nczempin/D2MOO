#pragma once

#include <Windows.h>
#include <cstdint>

// 1.12a Image Base: 0x6FA20000
constexpr int D2MCPClientImageBase = 0x6FA20000;

D2MCPCLIENT_DLL_DECL void __cdecl D2MCPClientCloseMCP();

D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10000();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10001();
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10002();
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10003();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10004(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10005();
D2MCPCLIENT_DLL_DECL void* __stdcall D2MCPClient_10006();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10007(int32_t a1, int32_t a2, int32_t a3);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10008(int32_t a1);
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClient_10009(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10010(const char* szName);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10011(const char* szName);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10012(int32_t a1);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10013(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10014();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10015(const char* szSrc, int32_t nMax);
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClient_10016(int32_t a1);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10017(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10018();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10019();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10020(int32_t a1);
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClient_10021(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10022(const char* a1, const char* a2);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10023();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10025(const char* a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10026();
D2MCPCLIENT_DLL_DECL uint16_t __stdcall D2MCPClient_10027();
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10028();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10029();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10030(const char* a1, int32_t a2);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10031(const char* a1, const char* a2, int32_t a3);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10032(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10033();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10034();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10035(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10036();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10037(const void* pData);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10038(const char* a1, uint16_t a2);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10039(int32_t a1, int32_t a2, int32_t a3);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10040();
D2MCPCLIENT_DLL_DECL void* __stdcall D2MCPClient_10041();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10042();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10043(const char* a1, uint16_t a2);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10044(char* pDst, const char* pSrc, int32_t nMax);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10045();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10046(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10047();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10048(int32_t a1);
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10049();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10050(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10051();
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10052();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10053(int32_t a1);
D2MCPCLIENT_DLL_DECL void __fastcall D2MCPClient_10054(int32_t a1, int32_t a2);
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10055(const char* a1, uint16_t a2);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10056();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10057(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10058(int32_t a1);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10059();
D2MCPCLIENT_DLL_DECL int32_t __fastcall D2MCPClient_10060(uint8_t a1, uint16_t a2);
D2MCPCLIENT_DLL_DECL int32_t __stdcall D2MCPClient_10061();
D2MCPCLIENT_DLL_DECL void __stdcall D2MCPClient_10062(int32_t a1);
