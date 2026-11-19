#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_grudge_sprite_context.h"

struct BattleAnimSystem;

enum XYTransformParam {
    XY_PARAM_STEPS = 0,
    XY_PARAM_STEP_SIZE,
    XY_PARAM_STEP_SIZE_X = 1,
    XY_PARAM_STEP_SIZE_Y,
    XY_PARAM_CUR_X,
    XY_PARAM_CUR_Y,

    XY_PARAM_SHAKE_COUNTER = 0,
    XY_PARAM_SHAKE_INTERVAL,
    XY_PARAM_SHAKE_REMAINING,
    XY_PARAM_SHAKE_CYCLE,
    XY_PARAM_SHAKE_EXTENT_X,
    XY_PARAM_SHAKE_EXTENT_Y,
    XY_PARAM_SHAKE_PREV_X,
    XY_PARAM_SHAKE_PREV_Y,

    XY_PARAM_REV_STEPS = 0,
    XY_PARAM_REV_CUR_X,
    XY_PARAM_REV_RADIUS_X,
    XY_PARAM_REV_CUR_Y,
    XY_PARAM_REV_RADIUS_Y,
    XY_PARAM_REV_STEP_SIZE_X,
    XY_PARAM_REV_STEP_SIZE_Y,
};

void ov07_022220FC(XYTransformContext *ctx, u16 sx, u16 ex, u16 sy, u16 ey, fx32 rx, fx32 ry, int steps);

static void ov07_0222FBEC(GrudgeSpriteContext *ctx, int dir);

static void ov07_0222FBEC(GrudgeSpriteContext *ctx, int dir) {
    int angleStep = (((360) * 0xFFFF) / 360) / 6;

    for (int i = 0; i < 6; i++) {
        ov07_022220FC(
            &ctx->revs[i],
            (((0) * 0xFFFF) / 360),
            (((180) * 0xFFFF) / 360),
            0,
            0,
            ((fx32)(((50) > 0) ? ((50) * (1 << 12) + 0.5f) : ((50) * (1 << 12) - 0.5f))),
            0,
            48);
        ctx->revs[i].data[XY_PARAM_REV_CUR_X] += angleStep * i;
        ctx->revs[i].data[XY_PARAM_REV_STEP_SIZE_X] *= dir;
    }
}

int ov07_0221FAE8(BattleAnimSystem *system);

BOOL ov07_02222180(XYTransformContext *ctx);

static void ov07_0222FC44(GrudgeSpriteContext *ctx);

static void ov07_0222FC44(GrudgeSpriteContext *ctx) {
    for (int i = 0; i < 6; i++) {
        ov07_02222180(&ctx->revs[i]);

        u16 angle = ctx->revs[i].data[XY_PARAM_REV_CUR_X] * 5;
        s16 offsetY = FX_MulInline(FX_SinIdx(angle), ((fx32)(((10) > 0) ? ((10) * (1 << 12) + 0.5f) : ((10) * (1 << 12) - 0.5f))) * ctx->dir) >> 12;

        ManagedSprite_SetPositionXY(ctx->flameSprites[i], ctx->attackerX + ctx->revs[i].x, ctx->attackerY + offsetY);

        if (ctx->revs[i].data[XY_PARAM_REV_CUR_X] >= (((90) * 0xFFFF) / 360)
            && ctx->revs[i].data[XY_PARAM_REV_CUR_X] <= (((269) * 0xFFFF) / 360)) {
            ManagedSprite_SetPriority(ctx->flameSprites[i], 1);
        } else {
            ManagedSprite_SetPriority(ctx->flameSprites[i], ov07_0221FAE8(ctx->battleAnimSys) + 1);
        }
    }
}

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

enum GrudgeSpriteState {
    GRUDGE_SPRITE_STATE_INIT = 0,
    GRUDGE_SPRITE_STATE_REVOLVE_AND_FADE_IN,
    GRUDGE_SPRITE_STATE_REVOLVE,
    GRUDGE_SPRITE_STATE_FADE_OUT,
    GIUDGE_SPRITE_STATE_CLEANUP,
};

void ov07_0222FD14(SysTask *task, void *param);

void ov07_0222FD14(SysTask *task, void *param) {
    GrudgeSpriteContext *ctx = param;

    switch (ctx->state) {
    case GRUDGE_SPRITE_STATE_INIT:
        ov07_02231E08(
            ctx->battleAnimSys,
            1,
            16 - 1);
        ov07_02222AC4(
            &ctx->alpha,
            1,
            16,
            16 - 1,
            16 - 16,
            10);
        ov07_0222FBEC(ctx, ctx->dir);
        ov07_0222FC44(ctx);
        ctx->state++;
        break;
    case GRUDGE_SPRITE_STATE_REVOLVE_AND_FADE_IN:
        ov07_0222FC44(ctx);

        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
            ctx->delay = 48 - (2 * 10);
        }
        break;
    case GRUDGE_SPRITE_STATE_REVOLVE:
        ov07_0222FC44(ctx);
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->state++;
            ov07_02222AC4(
                &ctx->alpha,
                16,
                1,
                16 - 16,
                16 - 1,
                10);
        }
        break;
    case GRUDGE_SPRITE_STATE_FADE_OUT:
        ov07_0222FC44(ctx);

        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
        }
        break;
    case GIUDGE_SPRITE_STATE_CLEANUP:
        for (int i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(ctx->flameSprites[i]);
        }

        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
