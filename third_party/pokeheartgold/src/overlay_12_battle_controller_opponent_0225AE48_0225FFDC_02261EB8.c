#include "global.h"

#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "player_data.h"

void ov12_02261EB8(BattleSystem *arg0);

void ov12_02261EB8(BattleSystem *battleSystem) {
    ov12_0223BFFC(battleSystem, 1U);
    BattleController_TryEmitExitRecording(battleSystem, BattleSystem_GetBattleContext(battleSystem));
}

void ov12_02261ED4(BattleSystem *arg0);

void ov12_02261ED4(BattleSystem *battleSystem) {
    ov12_0223BFFC(battleSystem, 2U);
    BattleController_TryEmitExitRecording(battleSystem, BattleSystem_GetBattleContext(battleSystem));
}

u8 ov12_02261EF0(BattleSystem *arg0, s32 arg1, u8 arg2);

u8 ov12_02261EF0(BattleSystem *battleSystem, s32 a1, u8 arg2) {
    u8 v0;
    u8 v1;

    v1 = arg2;
    if (!(BattleSystem_GetBattleType(battleSystem) & 4) || ((v1 != 0) && (v1 != 1))) {
        return v1;
    }
    v0 = PlayerProfile_GetVersion(BattleSystem_GetPlayerProfile(battleSystem, a1));
    switch (v0) {
    case 0:
        v1 += 0x7D;
        break;
    case 12:
        v1 += 0x7F;
        break;
    }
    return v1;
}
