#include "global.h"

#include "battle/trainer_ai.h"
#include "overlay_10_trainer_ai.h"

void ov10_0221CB64(BattleSystem *battleSys, BattleContext *ctx) {
    ov10_0221EF24(ctx, 1);
    ctx->trainerAIData.unk8 = ctx->totalTurns;
}
