#include "global.h"

#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_afterimage_context.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_memento_context.h"
#include "overlay_07_unused_sprite_context.h"

#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"

struct BattleAnimSystem;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

BOOL ov07_0221FB78(BattleAnimSystem *system, int battlerRole);

void ov07_0222F764(SysTask *task, void *param);

void ov07_0222F848(BattleAnimSystem *system);

void ov07_0222F848(BattleAnimSystem *system) {
    MementoContext *ctx = ov07_022324D8(system, sizeof(MementoContext));

    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);
    ctx->paletteData = ov07_0221FA78(ctx->battleAnimSys);

    Pokepic *attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    s16 attackerX = Pokepic_GetAttr(attackerSprite, POKEPIC_X);
    s16 attackerY = Pokepic_GetAttr(attackerSprite, POKEPIC_Y);
    attackerY -= Pokepic_GetAttr(attackerSprite, POKEPIC_SHADOW_H);

    Pokepic *defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
    s16 defenderX = Pokepic_GetAttr(defenderSprite, POKEPIC_X);
    s16 defenderY = Pokepic_GetAttr(defenderSprite, POKEPIC_Y);
    defenderY -= Pokepic_GetAttr(defenderSprite, POKEPIC_SHADOW_H);

    ctx->attackerY = attackerY;
    ctx->attackerHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerSprite = ov07_0221C4E8(ctx->battleAnimSys, 0);

    ManagedSprite_SetDrawPriority(ctx->attackerSprite, 100);
    ManagedSprite_SetPriority(ctx->attackerSprite, 1);
    ManagedSprite_SetPositionXY(ctx->attackerSprite, attackerX, attackerY);
    ManagedSprite_SetDrawFlag(ctx->attackerSprite, 0);
    ManagedSprite_SetOamMode(ctx->attackerSprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(ctx->attackerSprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    ctx->defenderSprite = ov07_0221C4E8(ctx->battleAnimSys, 1);

    ManagedSprite_SetDrawPriority(ctx->defenderSprite, 100);
    ManagedSprite_SetPriority(ctx->defenderSprite, 1);
    ManagedSprite_SetPositionXY(ctx->defenderSprite, defenderX, defenderY);
    ManagedSprite_SetDrawFlag(ctx->defenderSprite, 0);
    ManagedSprite_SetOamMode(ctx->defenderSprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAffineOverwriteMode(ctx->defenderSprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetAffineTranslation(ctx->defenderSprite, 0, (10 * 8) / 2);

    ctx->attackerFlipped = ov07_0221FB78(ctx->battleAnimSys, 0);
    ctx->defenderFlipped = ov07_0221FB78(ctx->battleAnimSys, 1);
    ctx->attackerPartnerSprite = ov07_0221C4E8(ctx->battleAnimSys, 2);
    ctx->defenderPartnerSprite = ov07_0221C4E8(ctx->battleAnimSys, 3);

    ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, 0);
    ManagedSprite_SetDrawFlag(ctx->defenderPartnerSprite, 0);

    ov07_0221C410(ctx->battleAnimSys, ov07_0222F764, ctx);
}

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

void ov07_02222864(AfterimageContext *ctx, XYTransformContext *transformCtx, XYTransformFunc transformFunc, s16 x, s16 y, u16 delay, u8 count, u8 mode, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3);

static void ov07_0222F9B8(UnusedSpriteContext *ctx);

static void ov07_0222F9B8(UnusedSpriteContext *ctx) {
    ov07_02222508(&ctx->scale, 25, 10, 10, 10);
    ov07_02222864(
        &ctx->afterImage,
        &ctx->scale,
        ov07_02222558,
        ctx->defenderX,
        ctx->defenderY,
        9,
        3,
        1,
        ctx->sprites[2],
        ctx->sprites[1],
        ctx->sprites[0],
        NULL);
}

static void ov07_0222FA08(UnusedSpriteContext *ctx);

static void ov07_0222FA08(UnusedSpriteContext *ctx) {
    ov07_02222508(&ctx->scale, 10, 10, 25, 6);
    ov07_02222864(
        &ctx->afterImage,
        &ctx->scale,
        ov07_02222558,
        ctx->defenderX,
        ctx->defenderY,
        5,
        3,
        1,
        ctx->sprites[0],
        ctx->sprites[1],
        ctx->sprites[2],
        NULL);
    ManagedSprite_SetDrawFlag(ctx->sprites[0], TRUE);
}

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_0222283C(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

BOOL ov07_02222914(AfterimageContext *ctx);

void ov07_0222FA64(SysTask *param0, void *param);

void ov07_0222FA64(SysTask *param0, void *param) {
    UnusedSpriteContext *ctx = param;

    switch (ctx->state) {
    case 0:
        ov07_0222F9B8(ctx);
        ctx->state++;
        break;
    case 1:
        if (ov07_02222914(&ctx->afterImage) == 0) {
            ManagedSprite_SetDrawFlag(ctx->sprites[0], 1);
            ctx->state++;
            ov07_022227A8(&ctx->shake, 4, 0, 1, 6);
        }
        break;
    case 2:
        if (ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite) == 0) {
            ctx->state++;
            ov07_0222FA08(ctx);
        }
        break;
    case 3:
        if (ov07_02222914(&ctx->afterImage) == 0) {
            ctx->state++;
        }
        break;
    case 4:
        for (int i = 0; i < 3; i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, param0);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
