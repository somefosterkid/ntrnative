#include "global.h"

#include "battle/trainer_ai.h"

void ov10_0221CEA4(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (iVar1 == ctx->trainerAIData.unk8) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221CED4(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (iVar1 != ctx->trainerAIData.unk8) {
        ov10_0221EF24(ctx, uVar2);
    }
}
