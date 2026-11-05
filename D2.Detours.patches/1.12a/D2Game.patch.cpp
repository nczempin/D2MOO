#include <DetoursPatch.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmicrosoft-cast"
#endif

extern "C" {
    __declspec(dllexport)
    constexpr int __cdecl GetBaseOrdinal() { return 10'001; }
    __declspec(dllexport)
    constexpr int __cdecl GetLastOrdinal() { return 10'061; }
    constexpr int GetOrdinalCount() { return GetLastOrdinal() - GetBaseOrdinal() + 1; }
}

static PatchAction patchActions[GetOrdinalCount()] = {
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10041_TASK_Create                      @10001 (1.10f @10041)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReceiveDatabaseCharacter                 @10002 (1.10f @10007)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetMemoryUsage                           @10003 (1.10f @10052)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_UpdateGamesProgress                      @10004 (1.10f @10004)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10024_RemoveClientFromGame             @10005 (1.10f @10024)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_112a_10006                             @10006 (1.10f @None)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10049_Return1                          @10007 (1.10f @10049)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10018                                  @10008 (1.10f @10018)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10013                                  @10009 (1.10f @10013)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10044                                  @10010 (1.10f @10044)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToAllClients                  @10011 (1.10f @10021)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetUnitsDescriptions                     @10012 (1.10f @10019)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetInitSeed                              @10013 (1.10f @10010)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetFreeMonsterIndex                     @10014 (1.10f @10027)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetStatistics                            @10015 (1.10f @10020)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_UpdateClients                            @10016 (1.10f @10005)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10043                                  @10017 (1.10f @10043)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameInformation                       @10018 (1.10f @10014)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CloseAllGames                            @10019 (1.10f @10006)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_CreateNewEmptyGame                       @10020 (1.10f @10047)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ReturnArgument                           @10021 (1.10f @10062)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SendMessageToGameClients                 @10022 (1.10f @10022)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsInfo                       @10023 (1.10f @10017)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10060_Return                           @10024 (1.10f @10060)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10029_SAVE_WriteFileInterface          @10025 (1.10f @10029)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10001_Return0                          @10026 (1.10f @10001)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10051                                  @10027 (1.10f @10051)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetAct                                  @10028 (1.10f @10026)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGameServerGameIds                     @10029 (1.10f @10015)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10048_Return1                          @10030 (1.10f @10048)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetGamesCount                            @10031 (1.10f @10012)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetServerCallbackFunctions               @10032 (1.10f @10023)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetRoomBySubtileCoordinates             @10033 (1.10f @10028)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_InitGameDataTable                        @10034 (1.10f @10002)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_GetPlayerUnitsCount                      @10035 (1.10f @10016)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10050                                  @10036 (1.10f @10050)
    PatchAction::FunctionReplacePatchByOriginal,     //   PLAYER_SetVirtualPlayerCount                  @10037 (1.10f @10059)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_112a_10038                             @10038 (1.10f @None)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10056                                  @10039 (1.10f @10056)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckFirstPickedSet                    @10040 (1.10f @10037)
    PatchAction::FunctionReplacePatchByOriginal,     //   CLIENTS_GetExpansionClientCount               @10041 (1.10f @10058)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10011                                  @10042 (1.10f @10011)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10057                                  @10043 (1.10f @10057)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10042                                  @10044 (1.10f @10042)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2GAME_10036_PLRSAVE_EnableSaveFileWriting    @10045 (1.10f @10036)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10046                                  @10046 (1.10f @10046)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_DisconnectClientByName                   @10047 (1.10f @10055)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_DisableSequences                       @10048 (1.10f @10035)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_ProcessGame                              @10049 (1.10f @10045)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10008                                  @10050 (1.10f @10008)
    PatchAction::FunctionReplacePatchByOriginal,     //   QUESTS_CheckNotIntroQuest                     @10051 (1.10f @10038)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10034_Return                           @10052 (1.10f @10034)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_SetGlobalAct                             @10053 (1.10f @10009)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_InitializeClock                          @10054 (1.10f @10039)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10061_Return                           @10055 (1.10f @10061)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10053                                  @10056 (1.10f @10053)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_10054                                  @10057 (1.10f @10054)
    PatchAction::FunctionReplacePatchByOriginal,     //   GAME_ProcessNetworkMessages                   @10058 (1.10f @10003)
    PatchAction::FunctionReplacePatchByOriginal,     //   TASK_FreeAllQueueSlots                        @10059 (1.10f @10040)
    PatchAction::FunctionReplacePatchByOriginal,     //   DEBUG_GetUnitFromTypeAndGUID                  @10060 (1.10f @10025)
    PatchAction::FunctionReplacePatchByOriginal,     //   D2Game_112a_10061                             @10061 (1.10f @None)
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
