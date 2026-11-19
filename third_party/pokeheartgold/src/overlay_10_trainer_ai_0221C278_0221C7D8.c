#include "global.h"

#include "battle/battle_system.h"
#include "battle/trainer_ai.h"

void ov10_0221C7D8(BattleSystem *battleSystem, BattleContext *ctx) {
    s32 v0;
    s32 v1;
    s32 v2;

    ov10_0221EF24(ctx, 1);
    v1 = ov10_0221EEF0(ctx);
    v2 = ov10_0221EEF0(ctx);
    v0 = ov10_0221EEF0(ctx);
    if (ctx->fieldSideConditionFlags[BattleSystem_GetFieldSide(battleSystem, ov10_0221EF34(ctx, (u8)v1))] & v2) {
        ov10_0221EF24(ctx, v0);
    }
}

void ov10_0221C828(BattleSystem *battleSystem, BattleContext *ctx) {
    s32 v0;
    s32 v1;
    s32 v2;

    ov10_0221EF24(ctx, 1);
    v1 = ov10_0221EEF0(ctx);
    v2 = ov10_0221EEF0(ctx);
    v0 = ov10_0221EEF0(ctx);
    if (!(ctx->fieldSideConditionFlags[BattleSystem_GetFieldSide(battleSystem, ov10_0221EF34(ctx, (u8)v1))] & v2)) {
        ov10_0221EF24(ctx, v0);
    }
}
