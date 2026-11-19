#include "global.h"

#include "overlay_07_faint_attack_context.h"
#include "overlay_07_battle_anim_funcs.h"

#include "pokepic.h"
#include "sprite_system.h"

s32 ov07_02222A44(void *, s32, s32);

s32 ov07_02231E08(void *, s32, s32);

void ov07_0222DB14(BattleAnimSystem *system);

void ov07_0222DB14(BattleAnimSystem *system) {
    ManagedSprite *managedSprite;
    Pokepic *pokepic;
    s32 dir;
    FaintAttackContext *ctx;

    ctx = ov07_022324D8(system, sizeof(FaintAttackContext));
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerSprite = pokepic;
    ctx->attackerX = (s16)Pokepic_GetAttr(pokepic, 0);
    ctx->attackerY = (s16)Pokepic_GetAttr(ctx->attackerSprite, 1);
    ctx->attackerShadowHeight = (s16)Pokepic_GetAttr(ctx->attackerSprite, 0x29);
    ctx->attackerY = (s16)(ctx->attackerY + 8);
    managedSprite = ov07_0221C4E8(ctx->battleAnimSys, 0);
    ctx->sprite = managedSprite;
    ManagedSprite_SetOamMode(managedSprite, GX_OAM_MODE_XLU);
    Pokepic_SetAttr(ctx->attackerSprite, 6, 1);
    ov07_02222A44(&ctx->revs, 2, 0x10);
    ov07_02231E08(ctx->battleAnimSys, 0x10, 0);
    ov07_02222AC4(&ctx->alpha, 0x10, 0, 0, 0x10, 0x20);
    ctx->revs.data[2] = (s32)(ctx->revs.data[2] * dir);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222DA60, ctx);
    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}
