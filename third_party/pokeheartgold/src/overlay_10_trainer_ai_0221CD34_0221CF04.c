#include "global.h"

#include "battle/overlay_12_0224E4FC.h"

s32 ov10_0221EEF0(BattleContext *);

s32 ov10_0221EF24(BattleContext *, s32);

void ov10_0221CF04(BattleSystem *arg0, BattleContext *arg1);

void ov10_0221CF04(BattleSystem *battleSystem, BattleContext *ctx) {
    s32 v0;
    s32 v1;

    ov10_0221EF24(ctx, 1);
    v0 = ov10_0221EEF0(ctx);
    v1 = ov10_0221EEF0(ctx);
    if (v0 == CheckSortSpeed(battleSystem, ctx, (s32)ctx->trainerAIData.battlerIdAttacker, (s32)ctx->trainerAIData.battlerIdTarget, 1)) {
        ov10_0221EF24(ctx, v1);
    }
}

void ov10_0221CF48(BattleSystem *arg0, BattleContext *arg1);

void ov10_0221CF48(BattleSystem *battleSystem, BattleContext *ctx) {
    s32 v0;
    s32 v1;

    ov10_0221EF24(ctx, 1);
    v0 = ov10_0221EEF0(ctx);
    v1 = ov10_0221EEF0(ctx);
    if (v0 != CheckSortSpeed(battleSystem, ctx, (s32)ctx->trainerAIData.battlerIdAttacker, (s32)ctx->trainerAIData.battlerIdTarget, 1)) {
        ov10_0221EF24(ctx, v1);
    }
}
