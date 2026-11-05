#include <D2PacketDef.h>
#include <cstddef>

static_assert(sizeof(D2GSPacketClt32) == 0x11, "size mismatch");
static_assert(sizeof(D2GSPacketClt3A) == 0x03, "size mismatch");
static_assert(sizeof(D2GSPacketClt42) == 0x01, "size mismatch");
static_assert(sizeof(D2GSPacketClt66) == 0x2E, "size mismatch");
#if defined(D2_VERSION_112A)
static_assert(sizeof(D2GSPacketClt67) == 0x25, "size mismatch");
static_assert(offsetof(D2GSPacketClt67, nLocale) == 0x14, "offset mismatch");
static_assert(offsetof(D2GSPacketClt67, szClientName) == 0x15, "offset mismatch");
static_assert(sizeof(D2GSPacketClt6C) == 0x0D, "size mismatch");
#else
static_assert(sizeof(D2GSPacketClt67) == 0x1D, "size mismatch");
static_assert(sizeof(D2GSPacketClt6C) == 0x09, "size mismatch");
#endif

static_assert(sizeof(D2GSPacketSrv7B) == 0x08, "size mismatch");
