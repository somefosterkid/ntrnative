#include "global.h"
struct BattleCastleOpponentApp;

#include "message_format.h"
#include "player_data.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02244ABC(struct BattleCastleOpponentApp *arg0, u32 arg1);

void ov83_02244ABC(BattleCastleOpponentApp *arg0, u32 idx) {
    BufferPlayersName(arg0->strTemplate, idx, Save_PlayerData_GetProfile(arg0->saveData));
}
