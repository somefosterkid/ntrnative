#include "global.h"

#include "battle/trainer_ai.h"

void ov10_0221C878(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (ctx->trainerAIData.unk8 < iVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C8A8(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;
    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if ((ctx->trainerAIData.unk8) > iVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C8D8(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (ctx->trainerAIData.unk8 == iVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C908(BattleSystem *battleSys, BattleContext *ctx) {
    s32 iVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    iVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (ctx->trainerAIData.unk8 != iVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C938(BattleSystem *battleSys, BattleContext *ctx) {
    u32 uVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    uVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if ((ctx->trainerAIData.unk8 & uVar1) != 0) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C968(BattleSystem *battleSys, BattleContext *ctx) {
    u32 uVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    uVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if ((ctx->trainerAIData.unk8 & uVar1) == 0) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C998(BattleSystem *battleSys, BattleContext *ctx) {
    u32 uVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    uVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (ctx->trainerAIData.unk2 == uVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}

void ov10_0221C9C8(BattleSystem *battleSys, BattleContext *ctx) {
    u32 uVar1;
    u32 uVar2;

    ov10_0221EF24(ctx, 1);
    uVar1 = ov10_0221EEF0(ctx);
    uVar2 = ov10_0221EEF0(ctx);
    if (ctx->trainerAIData.unk2 != uVar1) {
        ov10_0221EF24(ctx, uVar2);
    }
}
