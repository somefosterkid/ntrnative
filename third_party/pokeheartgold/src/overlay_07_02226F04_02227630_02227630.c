#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_bulk_up_context.h"
#include "pokepic.h"

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

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

s16 ov07_02222674(s16 baseY, int height, fx32 scaleY);

BOOL ov07_022227D8(XYTransformContext *ctx);

typedef struct StrengthContext {
    u8 state;
    s16 baseY;
    int spriteHeight;
    u8 blinkCounter;
    BattleAnimSystem *battleAnimSys;
    Pokepic *sprite;
    XYTransformContext scale;
    XYTransformContext shake;
} StrengthContext;

enum StrengthState {
    STRENGTH_STATE_SHRINK = 0,
    STRENGTH_STATE_FADE_TO_RED,
    STRENGTH_STATE_FADE_TO_NORMAL,
    STRENGTH_STATE_WAIT_FOR_FADE,
    STRENGTH_STATE_GROW,
    STRENGTH_STATE_CLEANUP,
};

static void ov07_02227630(SysTask *task, void *param);

static void ov07_02227630(SysTask *task, void *param) {
    StrengthContext *ctx = param;

    switch (ctx->state) {
    case STRENGTH_STATE_SHRINK:
        if (ov07_02222558(&ctx->scale) == 1) {
            ov07_022227D8(&ctx->shake);

            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            Pokepic_AddAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->shake.x);

            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ctx->state++;
        }
        break;
    case STRENGTH_STATE_FADE_TO_RED:
        Pokepic_StartPaletteFade(ctx->sprite, 0, 10, 0, (((0) << 10) | ((0) << 5) | (31)));
        ctx->state++;
        break;
    case STRENGTH_STATE_FADE_TO_NORMAL:
        if (Pokepic_ResumePaletteFade(ctx->sprite) == 0) {
            Pokepic_StartPaletteFade(ctx->sprite, 10, 0, 0, (((0) << 10) | ((0) << 5) | (31)));
            ctx->state++;
        }
        break;
    case STRENGTH_STATE_WAIT_FOR_FADE:
        if (Pokepic_ResumePaletteFade(ctx->sprite) == 0) {
            ctx->blinkCounter++;

            if (ctx->blinkCounter >= 3) {
                ov07_02222508(
                    &ctx->scale,
                    (s16)ov07_0221C4A8(ctx->battleAnimSys, 0),
                    100,
                    (s16)ov07_0221C4A8(ctx->battleAnimSys, 1),
                    ov07_0221C4A8(ctx->battleAnimSys, 3));
                ctx->state++;
            } else {
                ctx->state = STRENGTH_STATE_FADE_TO_RED;
            }
        }
        break;
    case STRENGTH_STATE_GROW:
        if (ov07_02222558(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ctx->state++;
        }
        break;
    case STRENGTH_STATE_CLEANUP:
        if (ov07_02222558(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY);
            ctx->state++;
        }
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

void ov07_02227824(BattleAnimSystem *system);

void ov07_02227824(BattleAnimSystem *system) {
    StrengthContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(StrengthContext));

    ctx->blinkCounter = 0;
    ctx->state = STRENGTH_STATE_SHRINK;
    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->baseY = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_Y_CENTER);
    ctx->spriteHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ov07_02222508(
        &ctx->scale,
        100,
        100,
        (s16)ov07_0221C4A8(system, 0),
        ov07_0221C4A8(system, 2));
    ov07_022227A8(&ctx->shake, 2, 0, 0, 10);
    ov07_0221C410(ctx->battleAnimSys, ov07_02227630, ctx);
}

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

enum BulkUpState {
    BULK_UP_STATE_SQUISH_Y = 0,
    BULK_UP_STATE_SQUISH_X,
    BULK_UP_STATE_RETURN_TO_NORMAL,
    BULK_UP_STATE_GROW,
    BULK_UP_STATE_SHRINK,
};

void ov07_022278B0(SysTask *task, void *param) {
    BulkUpContext *ctx = param;

    switch (ctx->state) {
    case BULK_UP_STATE_SQUISH_Y:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ov07_02222590(
                &ctx->scale,
                150,
                50,
                50,
                150,
                100,
                10);
            ctx->state++;
        }
        break;
    case BULK_UP_STATE_SQUISH_X:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ov07_02222590(
                &ctx->scale,
                50,
                100,
                150,
                100,
                100,
                5);
            ctx->state++;
        }
        break;
    case BULK_UP_STATE_RETURN_TO_NORMAL:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ov07_02222590(
                &ctx->scale,
                100,
                150,
                100,
                150,
                100,
                5);
            ctx->state++;
        }
        break;
    case BULK_UP_STATE_GROW:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            ov07_02222590(
                &ctx->scale,
                150,
                100,
                150,
                100,
                100,
                5);
            ctx->state++;
        }
        break;
    case BULK_UP_STATE_SHRINK:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->baseY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + offset);
        } else {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY);
            ctx->state++;
        }
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
