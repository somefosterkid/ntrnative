#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_value_lerp_context.h"

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


typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

typedef struct FlailContext {
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo shakeInfo;
    Pokepic *battlerSprites[2];
    s64 unused;
    XYTransformContext shake;
    ValueLerpContext angle;
} FlailContext;

enum FlailState {
    FLAIL_MODE_ATTACKER_STATE_SETUP_ROTATION = 0,
    FLAIL_MODE_ATTACKER_STATE_ROTATE,

    FLAIL_MODE_DEFENDER_STATE_SETUP_SHAKE = 0,
    FLAIL_MODE_DEFENDER_STATE_SHAKE,
};

enum {
    FLAIL_ANGLE_TABLE_ANGLE = 0,
    FLAIL_ANGLE_TABLE_FRAMES,
};

extern const int ov07_0223674C[5][2];

void ov07_02228E70(SysTask *task, void *param);

void ov07_02228E70(SysTask *task, void *param) {
    FlailContext *ctx = param;

    switch (ctx->common.state) {
    case FLAIL_MODE_ATTACKER_STATE_SETUP_ROTATION: {
        int startAngle;
        int endAngle;

        if (ctx->common.counter1 == 0) {
            startAngle = -ov07_0223674C[ctx->common.counter2][FLAIL_ANGLE_TABLE_ANGLE];
            endAngle = +ov07_0223674C[ctx->common.counter2][FLAIL_ANGLE_TABLE_ANGLE];
        } else {
            startAngle = +ov07_0223674C[ctx->common.counter2][FLAIL_ANGLE_TABLE_ANGLE];
            endAngle = -ov07_0223674C[ctx->common.counter2][FLAIL_ANGLE_TABLE_ANGLE];
        }

        ov07_022223F0(
            &ctx->angle,
            startAngle,
            endAngle,
            ov07_0223674C[ctx->common.counter2][FLAIL_ANGLE_TABLE_FRAMES]);

        if (ctx->common.counter1 == 1) {
            ctx->common.counter2++;
        }

        ctx->common.counter1 ^= 1;
        ctx->common.state++;
    }
    case FLAIL_MODE_ATTACKER_STATE_ROTATE:
        if (ov07_02222440(&ctx->angle) == 1) {
            Pokepic_SetAttr(
                ctx->battlerSprites[0],
                MON_SPRITE_ROTATION_Z,
                (u16)ctx->angle.value);
        } else {
            if (ctx->common.counter2 >= 5) {
                ctx->common.state++;
            } else {
                ctx->common.state--;
            }
        }
        break;
    default:
        Pokepic_SetAttr(ctx->battlerSprites[0], MON_SPRITE_ROTATION_Z, 0);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
