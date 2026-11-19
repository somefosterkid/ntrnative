#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_minimize_context.h"

#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

BOOL ov07_0221FB78(BattleAnimSystem *system, int battlerRole);

s32 ov07_02231E08(BattleAnimSystem *system, s32 spriteAlpha, s32 bgAlpha);

void ov07_02228834(SysTask *task, void *param);

void ov07_02228A50(BattleAnimSystem *system);

void ov07_02228A50(BattleAnimSystem *system) {
    ManagedSprite *managedSprite;
    Pokepic *pokepic;
    s32 i;
    s32 j;
    s8 dir;
    MinimizeContext *ctx;

    ctx = ov07_022324D8(system, sizeof(MinimizeContext));
    ctx->state = 0;
    ctx->cycles = 0;
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    pokepic = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerSprite = pokepic;
    ctx->attackerY = (s16)Pokepic_GetAttr(pokepic, POKEPIC_Y);
    ctx->attackerY = (s16)(ctx->attackerY - Pokepic_GetAttr(ctx->attackerSprite, POKEPIC_SHADOW_H));
    ctx->attackerSpriteHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_02231E08(ctx->battleAnimSys, -1, -1);
    reg_G2_BLDALPHA = 0x60C;
    for (i = 0; i < 3; i++) {
        ctx->sprites[i].state = 0;
        ctx->sprites[i].frameCounter = 0;
        managedSprite = ov07_0221C4E8(ctx->battleAnimSys, i);
        ctx->sprites[i].sprite = managedSprite;
        ManagedSprite_SetAffineOverwriteMode(managedSprite, 2U);
        ManagedSprite_SetOamMode(ctx->sprites[i].sprite, GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawPriority(ctx->sprites[i].sprite, i + 1);
    }
    if (ov07_0221FB78(system, 0) == 1) {
        dir = -1;
    } else {
        dir = 1;
    }
    ctx->spriteDir = dir;
    for (j = 0; j < 3; j++) {
        ManagedSprite_SetAffineScale(ctx->sprites[j].sprite, (f32)ctx->spriteDir, 1.0f);
    }
    ov07_0221C410(ctx->battleAnimSys, ov07_02228834, ctx);
}
