#include "global.h"

#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

s32 ov12_0223A7A0(void);

s32 ov12_02258DB0(BattleSystem *, OpponentData *, int, s32);

void ov12_02265FD4(UnkBattleSystemSub17C *, BattleSystem *, s32, enum Terrain);

void ov12_022389B8(BattleSystem *arg0);

void ov12_022389B8(BattleSystem *arg0) {
    BattleContext *sp0;
    enum Terrain temp_r0;
    s32 temp_r0_2;
    s32 var_r4;

    temp_r0 = BattleSystem_GetTerrainId(arg0);
    ov12_02265FD4(arg0->unk17C, arg0, 0, temp_r0);
    ov12_02265FD4(&arg0->unk17C[1], arg0, 1, temp_r0);
    sp0 = BattleSystem_GetBattleContext(arg0);
    var_r4 = 0;
    if ((s32)arg0->maxBattlers > 0) {
        do {
            temp_r0_2 = ov12_022581D4(arg0, sp0, 2, var_r4);
            ov12_02258DB0(arg0, arg0->opponentData[var_r4], BattleSystem_GetMonBall(arg0, BattleSystem_GetPartyMon(arg0, var_r4, temp_r0_2)), temp_r0_2);
            var_r4 += 1;
        } while (var_r4 < (s32)arg0->maxBattlers);
    }
    ov12_0223A7A0();
}
