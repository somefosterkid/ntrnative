#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_growth_context.h"

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


void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

BOOL Pokepic_ResumePaletteFade(Pokepic *monSprite);

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

enum GrowthState {
    GROWTH_STATE_SCALE_FADE = 0,
    GROWTH_STATE_WAIT,
};

enum {
    GROWTH_TABLE_SCALE_START = 0,
    GROWTH_TABLE_SCALE_END,
    GROWTH_TABLE_FRAMES,
    GROWTH_TABLE_COUNT
};

extern const u8 ov07_022366CC[2][GROWTH_TABLE_COUNT];

void ov07_02227FA4(SysTask *task, void *param);

void ov07_02227FA4(SysTask *task, void *param) {
    GrowthContext *ctx = param;

    switch (ctx->state) {
    case GROWTH_STATE_SCALE_FADE:
        ov07_02222508(
            &ctx->scale,
            ov07_022366CC[ctx->scaleState][GROWTH_TABLE_SCALE_START],
            100,
            ov07_022366CC[ctx->scaleState][GROWTH_TABLE_SCALE_END],
            ov07_022366CC[ctx->scaleState][GROWTH_TABLE_FRAMES]);

        if (ctx->scaleState == 0) {
            Pokepic_StartPaletteFade(ctx->sprite, 0, 6, 0, (((31) << 10) | ((31) << 5) | (31)));
        } else {
            Pokepic_StartPaletteFade(ctx->sprite, 6, 0, 0, (((31) << 10) | ((31) << 5) | (31)));
        }

        ctx->scaleState ^= 1;
        ctx->iterations++;
        ctx->state++;
        break;
    case GROWTH_STATE_WAIT:
        if (ov07_02222558(&ctx->scale) == 0 && Pokepic_ResumePaletteFade(ctx->sprite) == 0) {
            if (ctx->iterations < (2 * 2)) {
                ctx->state--;
            } else {
                ctx->state++;
            }
        }

        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
