#include "global.h"

#include "battle/trainer_ai.h"
#include "overlay_10_trainer_ai.h"

void ov10_0221E460(BattleSystem *battleSys, BattleContext *ctx) {
    s32 v0;

    ov10_0221EF24(ctx, 1);
    v0 = ov10_0221EEF0(ctx);
    if (ov10_0221FD34(battleSys, ctx, ctx->trainerAIData.battlerIdAttacker, 1) == 1) {
        ov10_0221EF24(ctx, v0);
    }
}
