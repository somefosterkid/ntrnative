#include "global.h"

#include "battle/trainer_ai.h"

void ov10_0221DEF0(BattleSystem *battleSys, BattleContext *ctx) {
    u32 uVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    uVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if ((ctx->fieldCondition & uVar1) != 0) {
        ov10_0221EF24(ctx, uVar2);
    }
}
