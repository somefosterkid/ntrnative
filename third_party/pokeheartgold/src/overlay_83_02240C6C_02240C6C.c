#include "global.h"
struct BattleCastleSelfApp;

#include "message_format.h"
#include "player_data.h"
#include "overlay_83_battle_castle_self_app.h"

void ov83_02240C6C(struct BattleCastleSelfApp *arg0, u32 arg1);

void ov83_02240C6C(BattleCastleSelfApp *arg0, u32 idx) {
    BufferPlayersName(arg0->strTemplate, idx, Save_PlayerData_GetProfile(arg0->saveData));
}
