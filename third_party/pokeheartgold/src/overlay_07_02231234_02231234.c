#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_grudge_sprite_context.h"

struct BattleAnimSystem;

typedef struct BurnSpriteContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    SpriteManager *spriteMan;
    ManagedSprite *sprites[6];
    int visibleFrames[6];
    int delays[6];
} BurnSpriteContext;

enum BurnSpriteState {
    BURN_SPRITE_STATE_ANIMATE = 0,
    BURN_SPRITE_STATE_CLEANUP,
};

BOOL ov07_02231204(ManagedSprite *sprite, int *visibleFrames, int *delay);

void ov07_02231234(SysTask *task, void *param);

void ov07_02231234(SysTask *task, void *param) {
    BurnSpriteContext *ctx = param;
    int i;

    switch (ctx->state) {
    case BURN_SPRITE_STATE_ANIMATE: {
        BOOL done;
        for (i = 0; i < 6; i++) {
            done = ov07_02231204(ctx->sprites[i], &ctx->visibleFrames[i], &ctx->delays[i]);
        }

        if (done) {
            ctx->state++;
        }
        break;
    }
    case BURN_SPRITE_STATE_CLEANUP:
        for (i = 0; i < 6; i++) {
            Sprite_DeleteAndFreeResources(ctx->sprites[i]);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

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

void ov07_022312A8(GrudgeSpriteContext *ctx, int dir);

void ov07_022312A8(GrudgeSpriteContext *ctx, int dir) {
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
