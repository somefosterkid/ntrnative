#include "global.h"

#include "overlay_07_growth_context.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_02228070(BattleAnimSystem *system);

void ov07_02228070(BattleAnimSystem *system) {
    GrowthContext *ctx;

    ctx = ov07_022324D8(system, sizeof(GrowthContext));
    ctx->state = 0;
    ctx->scaleState = 0;
    ctx->iterations = 0;
    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ov07_0221C410(ctx->battleAnimSys, ov07_02227FA4, ctx);
}
