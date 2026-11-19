#include "global.h"

#include "error_handling.h"
#include "overlay_80_0222AEF8.h"
#include "player_data.h"
#include "unk_02035900.h"
#include "overlay_80_battle_factory.h"

s32 ov80_0222B0E8(s32 arg0, s32 arg1, s32 arg2) {
    if (arg2 > 0x200) {
        GF_AssertFail();
    }
    return arg1 + 0x984 + (arg0 << 9);
}

s32 ov80_0222B108(BattleFactory *factory) {
    u16 *v0;

    v0 = factory->commBuffer;
    Save_PlayerData_GetProfile(factory->saveData);
    v0[1] = factory->tradeCount;
    v0[2] = factory->currentStreak;
    v0[3] = factory->currentRound;
    if (sub_02037030(0x16, v0, 0x3C) == 1) {
        return 1;
    }
    return 0;
}
