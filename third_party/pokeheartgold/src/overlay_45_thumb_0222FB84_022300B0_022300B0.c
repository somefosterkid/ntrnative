#include "global.h"

#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyVipRecord;

struct WiFiHistory;

struct WiFiList;

typedef enum PPW_LOBBY_STATS_RESULT {
    PPW_LOBBY_STATS_RESULT_SUCCESS,
    PPW_LOBBY_STATS_RESULT_INVALID_PARAM,
    PPW_LOBBY_STATS_RESULT_SERVER_ERROR
} PPW_LOBBY_STATS_RESULT;

typedef struct WiFiHistory WiFiHistory;

void ov45_0223040C(UnkStruct_ov66_022336C4_sub5 *param0, const PPW_LobbyVipRecord *lobbyVipRecord, u32 param2);

void ov45_022300B0(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyVipRecord *lobbyVipRecord, u32 param2);

void ov45_022300B0(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyVipRecord *lobbyVipRecord, u32 param2) {
    if (lobbyStatsResult == PPW_LOBBY_STATS_RESULT_SUCCESS) {
        ov45_0223040C(&_022577C0->unk_6A0, lobbyVipRecord, param2);
    } else {
        _022577C0->unk_132 = 1;
    }
}

void ov45_0223048C(UnkStruct_ov66_022336C4_sub6 *param0, const PPW_LobbyQuestionnaire *lobbyQuestionnaire);

void ov45_022300DC(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyQuestionnaire *lobbyQuestionnaire);

void ov45_022300DC(PPW_LOBBY_STATS_RESULT lobbyStatsResult, const PPW_LobbyQuestionnaire *lobbyQuestionnaire) {
    if (lobbyStatsResult == PPW_LOBBY_STATS_RESULT_SUCCESS) {
        ov45_0223048C(&_022577C0->unk_6A8, lobbyQuestionnaire);
    } else {
        _022577C0->unk_132 = 1;
    }
}
