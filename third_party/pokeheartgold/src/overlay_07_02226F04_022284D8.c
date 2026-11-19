#include "global.h"

#include "overlay_07_flash_context.h"
#include "overlay_07_battle_anim_funcs.h"

void ov07_022284D8(BattleAnimSystem *system);

void ov07_022284D8(BattleAnimSystem *system) {
    FlashContext *ctx;

    ctx = ov07_022324D8(system, sizeof(FlashContext));
    ctx->battleAnimSys = system;
    ctx->paletteData = ov07_0221FA78(system);
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_0221C410(ctx->battleAnimSys, ov07_022283D8, ctx);
}
