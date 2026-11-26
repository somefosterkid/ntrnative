#include "global.h"

#include "player_data.h"
#include "overlay_83_battle_castle_opponent_app.h"

u8 ov83_0224777C(SaveData *, u8, u8);

void ov83_02245104(BattleCastleOpponentApp *arg0, u16 arg1);

void ov83_02245104(BattleCastleOpponentApp *arg0, u16 arg1) {
    PlayerProfile *profile;
    u32 v0;

    profile = Save_PlayerData_GetProfile(arg0->saveData);
    *((s16 *)&arg0->commPayload[0]) = arg1;
    *((s16 *)&arg0->commPayload[1]) = (s16)PlayerProfile_GetTrainerGender(profile);
    v0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    do {
        arg0->commPayload[2 + v0] = ov83_0224777C(arg0->saveData, arg0->challengeType, v0);
        v0 += 1;
    } while ((s32)v0 < 3);
#else
    // the DS walks the commPayload[2..4] slots from (u8 *)app + 4 at byte offset 0x564 (offsetof commPayload)
    u8 *v1 = (u8 *)arg0 + 4;
    do {
        *(s16 *)(v1 + 0x564) = ov83_0224777C(arg0->saveData, arg0->challengeType, v0);
        v0 += 1;
        v1 += 2;
    } while ((s32)v0 < 3);
#endif
}
