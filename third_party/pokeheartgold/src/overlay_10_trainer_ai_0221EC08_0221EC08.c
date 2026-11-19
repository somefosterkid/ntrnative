#include "global.h"

#include "overlay_10_trainer_ai.h"
#include "battle/battle.h"

s32 ov10_0221EE28(BattleSystem *, BattleContext *, s32);

s32 ov10_0221EEF0(BattleContext *);

s32 ov10_0221EF24(BattleContext *, s32);

void ov10_0221EC08(BattleSystem *arg0, BattleContext *arg1) {
    ov10_0221EF24(arg1, 1);
    ov10_0221EE28(arg0, arg1, ov10_0221EEF0(arg1));
}
