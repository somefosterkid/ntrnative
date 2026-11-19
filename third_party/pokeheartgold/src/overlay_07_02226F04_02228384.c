#include "global.h"

#include "overlay_07_teleport_context.h"
#include "overlay_07_battle_anim_funcs.h"

#include "pokepic.h"

void ov07_02228384(BattleAnimSystem *system);

void ov07_02228384(BattleAnimSystem *system) {
    Pokepic *pokepic;
    TeleportContext *ctx;

    ctx = ov07_022324D8(system, sizeof(TeleportContext));
    ctx->state = 0;
    ctx->scaleType = 0;
    ctx->battleAnimSys = system;
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerSprite = pokepic;
    ctx->attackerY = (s16)Pokepic_GetAttr(pokepic, 1);
    ctx->baseY = ctx->attackerY;
    ctx->attackerHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_0221C410(ctx->battleAnimSys, ov07_022281C0, ctx);
}
