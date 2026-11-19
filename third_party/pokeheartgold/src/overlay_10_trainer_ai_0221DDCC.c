#include "global.h"

#include "battle/trainer_ai.h"
#include "overlay_10_trainer_ai.h"

void ov10_0221DDCC(BattleSystem *battleSys, BattleContext *ctx) {
    ov10_0221EF24(ctx, 1);
    ctx->trainerAIData.unk10 = ctx->trainerAIData.unk10 | 0xb;
}

void ov10_0221DDE8(void) {
}

void ov10_0221DDEC(void) {
}
