#include "global.h"

#include "overlay_07_megahorn_attacker_context.h"
#include "overlay_07_battle_anim_funcs.h"

#include "pokepic.h"

void ov07_02230BA0(BattleAnimSystem *system);

void ov07_02230BA0(BattleAnimSystem *system) {
    Pokepic *pokepic;
    s32 battler;
    MegahornAttackerContext *ctx;

    ctx = ov07_022324D8(system, sizeof(MegahornAttackerContext));
    ctx->battleAnimSys = system;
    battler = ov07_0221C468(system);
    pokepic = ov07_0221FA48(system, battler);
    ctx->sprite = pokepic;
    ctx->x = (s16)Pokepic_GetAttr(pokepic, 0);
    ctx->y = (s16)Pokepic_GetAttr(ctx->sprite, 1);
    ctx->dirX = ov07_02222004(system, battler);
    ctx->dirY = ov07_0222202C(system, battler);
    ov07_0221C410(ctx->battleAnimSys, ov07_02230A70, ctx);
}
