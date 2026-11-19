#include "global.h"

#include "overlay_07_meditate_context.h"
#include "overlay_07_battle_anim_funcs.h"

#include "pokepic.h"

void ov07_02228174(BattleAnimSystem *system);

void ov07_02228174(BattleAnimSystem *system) {
    Pokepic *pokepic;
    MeditateContext *ctx;

    ctx = ov07_022324D8(system, sizeof(MeditateContext));
    ctx->state = 0;
    ctx->iterations = 0;
    ctx->battleAnimSys = system;
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerSprite = pokepic;
    ctx->attackerY = (s16)Pokepic_GetAttr(pokepic, 1);
    ctx->attackerHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_0221C410(ctx->battleAnimSys, ov07_022280A8, ctx);
}
