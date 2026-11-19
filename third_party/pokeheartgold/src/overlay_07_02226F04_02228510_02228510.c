#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_splash_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);


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

enum BattleAnimAnchorType {
    BATTLE_ANIM_ANCHOR_BOTTOM = 0,
    BATTLE_ANIM_ANCHOR_TOP,
};

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222B4(XYTransformContext *ctx);

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

void ov07_022226C4(Pokepic *sprite, s16 y, s32 height, fx32 scale, enum BattleAnimAnchorType anchor);

enum SplashState {
    SPLASH_STATE_SETUP_ANIMS = 0,
    SPLASH_STATE_WAIT_FOR_ANIMS,
};

enum {
    SPLASH_TABLE_SCALE_SX = 0,
    SPLASH_TABLE_SCALE_EX,
    SPLASH_TABLE_SCALE_SY,
    SPLASH_TABLE_SCALE_EY,
    SPLASH_TABLE_SCALE_FRAMES,
    SPLASH_TABLE_SCALE_COUNT,

    SPLASH_TABLE_POS_SY = 0,
    SPLASH_TABLE_POS_EY,
    SPLASH_TABLE_POS_FRAMES,
    SPLASH_TABLE_POS_COUNT
};

extern const u8 ov07_022366F5[3][SPLASH_TABLE_SCALE_COUNT];

extern const u8 ov07_022366D8[3][SPLASH_TABLE_POS_COUNT];

void ov07_02228510(SysTask *task, void *param);

void ov07_02228510(SysTask *task, void *param) {
    SplashContext *ctx = param;

    switch (ctx->state) {
    case SPLASH_STATE_SETUP_ANIMS:
        ov07_02222590(
            &ctx->scale,
            ov07_022366F5[ctx->iteration][SPLASH_TABLE_SCALE_SX],
            ov07_022366F5[ctx->iteration][SPLASH_TABLE_SCALE_EX],
            ov07_022366F5[ctx->iteration][SPLASH_TABLE_SCALE_SY],
            ov07_022366F5[ctx->iteration][SPLASH_TABLE_SCALE_EY],
            100,
            ov07_022366F5[ctx->iteration][SPLASH_TABLE_SCALE_FRAMES]);

        ov07_02222268(
            &ctx->pos,
            0,
            0,
            ctx->attackerY + ov07_022366D8[ctx->iteration][SPLASH_TABLE_POS_SY],
            ctx->attackerY + ov07_022366D8[ctx->iteration][SPLASH_TABLE_POS_EY],
            ov07_022366D8[ctx->iteration][SPLASH_TABLE_POS_FRAMES]);

        ctx->iteration++;
        ctx->state++;
        break;
    case SPLASH_STATE_WAIT_FOR_ANIMS: {
        int done = 0;

        if (ov07_0222260C(&ctx->scale) == 0) {
            done++;
        }

        if (ov07_022222B4(&ctx->pos) == 0) {
            done++;
        }

        if (done >= 2) {
            if (ctx->iteration >= (sizeof(ov07_022366F5) / sizeof(*(ov07_022366F5)))) {
                ctx->cycles++;
                ctx->iteration = 0;

                if (ctx->cycles >= 3) {
                    ctx->state++;
                }
            } else {
                ctx->state = SPLASH_STATE_SETUP_ANIMS;
            }
        }

        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_X, ctx->scale.x);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
        ov07_022226C4(
            ctx->attackerSprite,
            ctx->pos.y,
            ctx->attackerHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);
        break;
    }
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
