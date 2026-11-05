#include <D2MCPClient.h>
#include <cstring>

// 1.12a Globals (0x6FA2AE84..0x6FA2B234)
static int32_t gdwMcp_6FA2AE84 = 0;
static int32_t gdwMcp_6FA2AEA4 = 0;
static uint16_t gwMcp_6FA2AEA8 = 0;
static int32_t gdwMcp_6FA2AEAC = 0;
static int32_t gdwMcp_6FA2AEB0 = 0;
static int32_t gdwMcp_6FA2AED8 = 0;
static int32_t gdwMcpSocket_6FA2B028 = 0;
static int32_t gdwMcp_6FA2B02C = 0;
static uint8_t gaMcpBuffer_6FA2B030[0x190] = {};
static int32_t gdwMcp_6FA2B1BC = 0;
static char gszMcp_6FA2B1C0[4] = {};
static int32_t gdwMcp_6FA2B1C4 = 0;
static int32_t gdwMcp_6FA2B1C8 = 0;
static int32_t gdwMcp_6FA2B1CC = 0;
static int32_t gdwMcp_6FA2B1D0 = 0;
static int32_t gdwMcp_6FA2B1D4 = 0;
static uint8_t gaMcp_6FA2B1F8[0x14] = {};
static int32_t gdwMcp_6FA2B20C = 0;
static int32_t gdwMcp_6FA2B214 = 0;
static int32_t gdwMcp_6FA2B21C = 0;
static int32_t gdwMcp_6FA2B220 = 0;
static int32_t gdwMcp_6FA2B224 = 0;
static int32_t gdwMcp_6FA2B228 = 0;
static int32_t gdwMcp_6FA2B22C = 0;
static int32_t gdwMcp_6FA2B230 = 0;
static int32_t gdwMcp_6FA2B234 = 0;

// 1.12a @10024: 0x6FA27390
void __cdecl D2MCPClientCloseMCP()
{
	gdwMcpSocket_6FA2B028 = 0;
	gdwMcp_6FA2B1D0 = 0;
}

int32_t __stdcall D2MCPClient_10000() { return gdwMcp_6FA2AED8; }
int32_t __fastcall D2MCPClient_10001() { return 0; }
int32_t __stdcall D2MCPClient_10002() { return gdwMcp_6FA2B02C; }
int32_t __stdcall D2MCPClient_10003() { return 0; }
void __stdcall D2MCPClient_10004(int32_t a1) { gdwMcp_6FA2AE84 = a1; }
int32_t __stdcall D2MCPClient_10005() { return gdwMcp_6FA2B21C; }
void* __stdcall D2MCPClient_10006() { return gaMcp_6FA2B1F8; }
int32_t __fastcall D2MCPClient_10007(int32_t a1, int32_t a2, int32_t a3) { (void)a1; (void)a2; (void)a3; gdwMcp_6FA2B1C8 = 0; return 0; }
int32_t __stdcall D2MCPClient_10008(int32_t a1) { (void)a1; return 0; }
void __fastcall D2MCPClient_10009(int32_t a1) { gdwMcp_6FA2B234 = a1; }
int32_t __fastcall D2MCPClient_10010(const char* szName) { (void)szName; gdwMcp_6FA2B1C8 = 0; return 0; }
int32_t __fastcall D2MCPClient_10011(const char* szName) { (void)szName; return 0; }
void __stdcall D2MCPClient_10012(int32_t a1) { gdwMcp_6FA2B214 = a1; }
void __stdcall D2MCPClient_10013(int32_t a1) { gdwMcp_6FA2B02C = a1; }
int32_t __stdcall D2MCPClient_10014() { return gdwMcp_6FA2B1C8; }
void __stdcall D2MCPClient_10015(const char* szSrc, int32_t nMax) { (void)szSrc; (void)nMax; }
void __fastcall D2MCPClient_10016(int32_t a1) { gdwMcp_6FA2B228 = a1; }
void __stdcall D2MCPClient_10017(int32_t a1) { gdwMcp_6FA2AEAC = a1; }
int32_t __stdcall D2MCPClient_10018() { return gdwMcp_6FA2AEAC; }
int32_t __fastcall D2MCPClient_10019() { return 0; }
void __stdcall D2MCPClient_10020(int32_t a1) { gdwMcp_6FA2B1D4 = a1; }
void __fastcall D2MCPClient_10021(int32_t a1) { gdwMcp_6FA2B224 = a1; }
int32_t __fastcall D2MCPClient_10022(const char* a1, const char* a2) { (void)a1; (void)a2; return 0; }
int32_t __fastcall D2MCPClient_10023() { return 0; }
int32_t __fastcall D2MCPClient_10025(const char* a1) { (void)a1; return 0; }
int32_t __stdcall D2MCPClient_10026() { return gdwMcp_6FA2B1C4; }
uint16_t __stdcall D2MCPClient_10027() { return gwMcp_6FA2AEA8; }
int32_t __stdcall D2MCPClient_10028() { return 0; }
int32_t __fastcall D2MCPClient_10029() { return 0; }
int32_t __fastcall D2MCPClient_10030(const char* a1, int32_t a2) { (void)a1; (void)a2; return 0; }
int32_t __fastcall D2MCPClient_10031(const char* a1, const char* a2, int32_t a3) { (void)a1; (void)a2; (void)a3; gdwMcp_6FA2B1C8 = 0; return 0; }
void __stdcall D2MCPClient_10032(int32_t a1) { gdwMcp_6FA2B20C = a1; }
int32_t __stdcall D2MCPClient_10033() { return gdwMcp_6FA2B1D4; }
void __stdcall D2MCPClient_10034() { std::memset(gaMcpBuffer_6FA2B030, 0, sizeof(gaMcpBuffer_6FA2B030)); }
void __stdcall D2MCPClient_10035(int32_t a1) { gdwMcp_6FA2B21C = a1; }
int32_t __stdcall D2MCPClient_10036() { return gdwMcp_6FA2B220; }
int32_t __fastcall D2MCPClient_10037(const void* pData) { (void)pData; return 0; }
int32_t __fastcall D2MCPClient_10038(const char* a1, uint16_t a2) { (void)a1; (void)a2; return 0; }
int32_t __stdcall D2MCPClient_10039(int32_t a1, int32_t a2, int32_t a3) { (void)a1; (void)a2; (void)a3; return 0; }
int32_t __stdcall D2MCPClient_10040() { return gdwMcp_6FA2B1BC; }
void* __stdcall D2MCPClient_10041() { return gaMcpBuffer_6FA2B030; }
int32_t __fastcall D2MCPClient_10042() { return 0; }
int32_t __fastcall D2MCPClient_10043(const char* a1, uint16_t a2) { (void)a1; (void)a2; return 0; }
void __stdcall D2MCPClient_10044(char* pDst, const char* pSrc, int32_t nMax) { (void)pDst; (void)pSrc; (void)nMax; (void)gszMcp_6FA2B1C0; }
int32_t __stdcall D2MCPClient_10045() { return gdwMcp_6FA2AE84; }
void __stdcall D2MCPClient_10046(int32_t a1) { gdwMcp_6FA2AEA4 = a1; }
int32_t __stdcall D2MCPClient_10047() { return gdwMcp_6FA2B214; }
void __stdcall D2MCPClient_10048(int32_t a1) { gdwMcp_6FA2B1C8 = a1; }
void __stdcall D2MCPClient_10049() { gdwMcp_6FA2B220 = 0; }
void __stdcall D2MCPClient_10050(int32_t a1) { gdwMcp_6FA2B1BC = a1; }
int32_t __stdcall D2MCPClient_10051() { return gdwMcp_6FA2AEB0; }
int32_t __stdcall D2MCPClient_10052() { return gdwMcp_6FA2B1D0; }
void __stdcall D2MCPClient_10053(int32_t a1) { gdwMcp_6FA2B1CC = a1; }
void __fastcall D2MCPClient_10054(int32_t a1, int32_t a2) { gdwMcp_6FA2B22C = a1; gdwMcp_6FA2B230 = a2; }
int32_t __fastcall D2MCPClient_10055(const char* a1, uint16_t a2) { (void)a1; (void)a2; return 0; }
int32_t __stdcall D2MCPClient_10056() { return gdwMcp_6FA2B1CC; }
int32_t __fastcall D2MCPClient_10057(int32_t a1) { (void)a1; return 0; }
int32_t __stdcall D2MCPClient_10058(int32_t a1) { (void)a1; return gdwMcp_6FA2B1D0; }
int32_t __stdcall D2MCPClient_10059() { return gdwMcp_6FA2AEA4; }
int32_t __fastcall D2MCPClient_10060(uint8_t a1, uint16_t a2) { (void)a1; (void)a2; return 0; }
int32_t __stdcall D2MCPClient_10061() { return gdwMcp_6FA2B20C; }
void __stdcall D2MCPClient_10062(int32_t a1) { gdwMcp_6FA2B1C4 = a1; }
