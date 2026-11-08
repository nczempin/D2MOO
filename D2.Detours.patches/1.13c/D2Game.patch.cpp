#include <DetoursPatch.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'000; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'060; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

static PatchAction patchActions[GetOrdinalCount()] = {
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10054                                  @10000 (1.12a @10057)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsInfo                       @10001 (1.12a @10023)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_ProcessGame                              @10002 (1.12a @10026)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10024_RemoveClientFromGame             @10003 (1.12a @10005)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10029_SAVE_WriteFileInterface          @10004 (1.12a @10025)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetFreeMonsterIndex                     @10005 (1.12a @10014)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CloseAllGames                            @10006 (1.12a @10019)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReturnArgument                           @10007 (1.12a @10039)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_UpdateGamesProgress                      @10008 (1.12a @10004)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameInformation                       @10009 (1.12a @10018)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10033                                  @10010 (1.12a @10000)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetMemoryUsage                           @10011 (1.12a @10003)
    PatchAction::FunctionReplacePatchByOriginal,     //   CLIENTS_GetExpansionClientCount               @10012 (1.12a @10041)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10001_Return0                          @10013 (1.12a @10045)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetUnitsDescriptions                     @10014 (1.12a @10012)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10042                                  @10015 (1.12a @10044)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_InitializeClock                          @10016 (1.12a @10054)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetInitSeed                              @10017 (1.12a @10053)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetAct                                  @10018 (1.12a @10028)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10036_PLRSAVE_EnableSaveFileWriting    @10019 (1.12a @10024)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10049_Return1                          @10020 (1.12a @10030)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10041_TASK_Create                      @10021 (1.12a @10049)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGamesCount                            @10022 (1.12a @10031)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10030_Return                           @10023 (1.12a @10001)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_UpdateClients                            @10024 (1.12a @10016)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10018                                  @10025 (1.12a @10008)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10008                                  @10026 (1.12a @10050)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckFirstPickedSet                    @10027 (1.12a @10051)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToGameClients                 @10028 (1.12a @10022)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_DisableSequences                       @10029 (1.12a @10048)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10034_Return                           @10030 (1.12a @10052)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10057                                  @10031 (1.12a @10043)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10032_Return0                          @10032 (1.12a @10032)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToAllClients                  @10033 (1.12a @10011)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_DisconnectClientByName                   @10034 (1.12a @10047)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReceiveDatabaseCharacter                 @10035 (1.12a @10002)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10044                                  @10036 (1.12a @10010)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_InitGameDataTable                        @10037 (1.12a @10034)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10046                                  @10038 (1.12a @10046)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetUnitFromTypeAndGUID                  @10039 (1.12a @10060)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ProcessNetworkMessages                   @10040 (1.12a @10058)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10031_Return                           @10041 (1.12a @10006)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetRoomBySubtileCoordinates             @10042 (1.12a @10033)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetGlobalAct                             @10043 (1.12a @10021)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CreateNewEmptyGame                       @10044 (1.12a @10020)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10013                                  @10045 (1.12a @10009)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10048_Return1                          @10046 (1.12a @10007)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10050                                  @10047 (1.12a @10036)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetServerCallbackFunctions               @10048 (1.12a @10038)
    PatchAction::FunctionReplacePatchByOriginal,     //   PLAYER_SetVirtualPlayerCount                  @10049 (1.12a @10037)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10051                                  @10050 (1.12a @10027)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckNotIntroQuest                     @10051 (1.12a @10040)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10056                                  @10052 (1.12a @10013)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsCount                      @10053 (1.12a @10035)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10053                                  @10054 (1.12a @10056)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10011                                  @10055 (1.12a @10042)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10043                                  @10056 (1.12a @10017)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10060_Return                           @10057 (1.12a @10055)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_FreeAllQueueSlots                        @10058 (1.12a @10059)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetStatistics                            @10059 (1.12a @10015)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameServerGameIds                     @10060 (1.12a @10029)
};

extern "C" {

__declspec(dllexport)
PatchAction __cdecl GetPatchAction(int ordinal)
{
#ifdef DISABLE_ALL_PATCHES
    return PatchAction::Ignore;
#else
    if (ordinal < GetBaseOrdinal() || ordinal > GetLastOrdinal())
        return PatchAction::FunctionReplacePatchByOriginal;

    static_assert(GetOrdinalCount() == (sizeof(patchActions) / sizeof(*patchActions)), "Make sure we have the right number of ordinal patch entries");
    return ::patchActions[ordinal - GetBaseOrdinal()];
#endif
}

static const int D2GameImageBase = 0x6FC20000;

static ExtraPatchAction extraPatchActions[] = {
    { 0, 0, PatchAction::Ignore},
};

__declspec(dllexport)
constexpr int __cdecl GetExtraPatchActionsCount() {
#ifdef DISABLE_ALL_PATCHES
    return 0;
#else
    return sizeof(extraPatchActions) / sizeof(ExtraPatchAction);
#endif
}

__declspec(dllexport)
ExtraPatchAction* __cdecl GetExtraPatchAction(int index)
{
    return &extraPatchActions[index];
}

}

#include <type_traits>
static_assert(std::is_same<decltype(GetBaseOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetLastOrdinal)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetPatchAction)*, GetPatchActionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetExtraPatchActionsCount)*, GetIntegerFunctionType>::value, "Ensure calling convention doesn't change");
static_assert(std::is_same<decltype(GetExtraPatchAction)*, GetExtraPatchActionType>::value, "Ensure calling convention doesn't change");
