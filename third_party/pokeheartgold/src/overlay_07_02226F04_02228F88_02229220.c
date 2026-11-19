#include "global.h"

#include "overlay_07_odor_sleuth_context.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_02229220(BattleAnimSystem *system);

void ov07_02229220(BattleAnimSystem *system) {
    OdorSleuthContext *ctx;

    ctx = ov07_022324D8(system, sizeof(OdorSleuthContext));
    ov07_02231FE4(system, &ctx->common);
    ctx->monSprites[0] = ov07_0221C4E8(ctx->common.battleAnimSys, 0);
    ctx->monSprites[1] = ov07_0221C4E8(ctx->common.battleAnimSys, 1);
    ctx->battlerSprites[0] = ov07_0221FA48(ctx->common.battleAnimSys, ov07_0221C468(ctx->common.battleAnimSys));
    ctx->battlerSprites[1] = ov07_0221FA48(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));
    ov07_02231FA0(ctx->battlerSprites[0], &ctx->spritePositions[0]);
    ov07_02231FA0(ctx->battlerSprites[1], &ctx->spritePositions[1]);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222915C, ctx);
}
