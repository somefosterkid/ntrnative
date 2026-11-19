#include "global.h"

#include "overlay_10_trainer_ai.h"
#include "battle/battle.h"

u32 ov10_0221EEF0(BattleContext *);

void ov10_0221EF24(BattleContext *, u32);

void ov10_0221EC28(u32 param_1, BattleContext *param_2) {
    u32 uVar1;

    ov10_0221EF24(param_2, 1);
    uVar1 = ov10_0221EEF0(param_2);
    ov10_0221EF24(param_2, uVar1);
    return;
}
