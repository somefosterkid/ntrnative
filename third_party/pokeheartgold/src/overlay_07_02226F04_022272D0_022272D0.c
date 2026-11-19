#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_bg_scroll_wave_context.h"

void ov07_022272D0(BattleAnimSystem *system);

void ov07_022272D0(BattleAnimSystem *system) {
    BgScrollWaveContext *ctx;

    ctx = ov07_022324D8(system, sizeof(BgScrollWaveContext));
    ov07_02231FE4(system, &ctx->common);
    ctx->frames = ov07_0221C4A8(system, 0);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_02227248, ctx);
}
