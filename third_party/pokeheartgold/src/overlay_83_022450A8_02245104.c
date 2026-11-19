#include "global.h"

#include "player_data.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_0224777C(SaveData *, u8, u8);

void ov83_02245104(void *arg0, u16 arg1);

void ov83_02245104(void *arg0, u16 arg1) {
    PlayerProfile *profile;
    u32 v0;
    void *v1;

    profile = Save_PlayerData_GetProfile(((BattleCastleOpponentApp *)arg0)->saveData);
    *((s16 *)&((BattleCastleOpponentApp *)arg0)->commPayload[0]) = arg1;
    *((s16 *)&((BattleCastleOpponentApp *)arg0)->commPayload[1]) = (s16)PlayerProfile_GetTrainerGender(profile);
    v0 = 0;
    v1 = arg0 + 4;
    do {
        *(s16 *)((u8 *)v1 + 0x564) = ov83_0224777C(((BattleCastleOpponentApp *)arg0)->saveData, ((BattleCastleOpponentApp *)arg0)->challengeType, v0);
        v0 += 1;
        v1 += 2;
    } while ((s32)v0 < 3);
}
