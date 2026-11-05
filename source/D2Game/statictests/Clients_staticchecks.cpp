
#include <GAME/Clients.h>

#if defined(D2_VERSION_110F)
static_assert(sizeof(D2ClientStrc) == 0x510, "Size of D2ClientStrc does not match v1.10f");
static_assert(sizeof(D2GameStrc) == 0x1DE8, "Size of D2GameStrc does not match v1.10f");
#elif D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 11
static_assert(sizeof(D2ClientStrc) == 0x518, "Size of D2ClientStrc does not match v1.11+");
static_assert(sizeof(D2GameStrc) == 0x1DF4, "Size of D2GameStrc does not match v1.11+");
#endif
