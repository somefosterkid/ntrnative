#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_scale_pokemon_sprite_context.h"

#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

BOOL ov07_0221FB78(BattleAnimSystem *system, int battlerRole);

s32 ov07_02231E08(BattleAnimSystem *system, s32 spriteAlpha, s32 bgAlpha);

void ov07_022241D8(SysTask *task, void *param);

void ov07_02224348(BattleAnimSystem *system);

void ov07_02224348(BattleAnimSystem *system) {
    ManagedSprite *managedSprite;
    Pokepic *pokepic;
    int height;
    s32 spriteAlpha;
    s32 flipRole;
    ScalePokemonSpriteContext *ctx;

    ctx = ov07_022324D8(system, sizeof(ScalePokemonSpriteContext));
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    if (ov07_0221C4A8(system, 0) == 0) {
        pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
        height = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    } else {
        pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
        height = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    }
    ctx->battlerHeight = height;
    ctx->baseY = (s16)Pokepic_GetAttr(pokepic, POKEPIC_Y);
    ctx->baseY = (s16)(ctx->baseY - Pokepic_GetAttr(pokepic, POKEPIC_SHADOW_H));
    ctx->spriteAlpha = ov07_0221C4A8(system, 1);
    ctx->startScale = ov07_0221C4A8(system, 2);
    ctx->endScale = ov07_0221C4A8(system, 3);
    ctx->refScale = ov07_0221C4A8(system, 4);
    ctx->cycles = ov07_0221C4A8(system, 5);
    ctx->scaleFrames = ov07_0221C4A8(system, 6);
    managedSprite = ov07_0221C4E8(ctx->battleAnimSys, ov07_0221C4A8(system, 7));
    ctx->sprite = managedSprite;
    ManagedSprite_SetDrawPriority(managedSprite, 0x64U);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, 2U);
    spriteAlpha = ctx->spriteAlpha;
    ov07_02231E08(ctx->battleAnimSys, spriteAlpha, 0x10 - spriteAlpha);
    ov07_0221C410(ctx->battleAnimSys, ov07_022241D8, ctx);
    if (ov07_0221C4A8(system, 0) != 0) {
        flipRole = 1;
    } else {
        flipRole = 0;
    }
    if (ov07_0221FB78(ctx->battleAnimSys, flipRole) == 1) {
        ctx->flip = 1;
    }
}
