
#include <QUESTS/Quests.h>

#if defined(D2_VERSION_110F) || defined(D2_VERSION_112A)
static_assert(sizeof(D2QuestDataStrc) == 0xF4, "Size of D2QuestDataStrc does not match v1.10f/v1.12a");
#endif
