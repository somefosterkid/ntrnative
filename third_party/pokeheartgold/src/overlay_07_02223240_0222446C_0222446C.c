#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "pokepic.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

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

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

void ov07_022226C4(Pokepic *sprite, s16 y, s32 height, fx32 scale, enum BattleAnimAnchorType anchor);

BOOL ov07_02222768(XYTransformContext *ctx, Pokepic *sprite);

BOOL ov07_02222788(XYTransformContext *ctx, Pokepic *sprite);

typedef struct ScaleBattlerSpriteContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    s16 battlerY;
    s16 battlerSpriteHeight;
    Pokepic *monSprite;
    XYTransformContext scale;
    int startX;
    int endX;
    int startY;
    int endY;
    int reference;
    int cycles;
    int scaleFrames;
    int hold;
} ScaleBattlerSpriteContext;

enum ScaleBattlerSpriteState {
    SCALE_BATTLER_SPRITE_STATE_INIT_SCALE = 0,
    SCALE_BATTLER_SPRITE_STATE_SCALE,
    SCALE_BATTLER_SPRITE_STATE_HOLD,
    SCALE_BATTLER_SPRITE_STATE_INIT_RESTORE,
    SCALE_BATTLER_SPRITE_STATE_RESTORE,
    SCALE_BATTLER_SPRITE_STATE_CLEANUP,
};

void ov07_02223224(Pokepic *sprite);

static void ov07_0222446C(SysTask *task, void *param);

static void ov07_0222446C(SysTask *task, void *param) {
    ScaleBattlerSpriteContext *ctx = param;

    switch (ctx->state) {
    case SCALE_BATTLER_SPRITE_STATE_INIT_SCALE:
        ov07_02222590(
            &ctx->scale,
            ctx->startX,
            ctx->endX,
            ctx->startY,
            ctx->endY,
            ctx->reference,
            ((ctx->scaleFrames) >> 16));
        ov07_02222768(&ctx->scale, ctx->monSprite);
        ctx->state++;
        break;
    case SCALE_BATTLER_SPRITE_STATE_SCALE:
        if (ov07_02222788(&ctx->scale, ctx->monSprite)) {
            ov07_022226C4(
                ctx->monSprite,
                ctx->battlerY,
                ctx->battlerSpriteHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->state++;
        }
        break;
    case SCALE_BATTLER_SPRITE_STATE_HOLD:
        if (ctx->hold == 0) {
            ctx->state++;
        } else {

            ctx->hold--;
            break;
        }
    case SCALE_BATTLER_SPRITE_STATE_INIT_RESTORE:
        ov07_02222590(
            &ctx->scale,
            ctx->endX,
            ctx->startX,
            ctx->endY,
            ctx->startY,
            ctx->reference,
            ((ctx->scaleFrames) & 0xFFFF));
        ov07_02222788(&ctx->scale, ctx->monSprite);
        ctx->state++;
        break;
    case SCALE_BATTLER_SPRITE_STATE_RESTORE:
        if (ov07_02222788(&ctx->scale, ctx->monSprite)) {
            ov07_022226C4(
                ctx->monSprite,
                ctx->battlerY,
                ctx->battlerSpriteHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->cycles--;

            if (ctx->cycles <= 0) {
                ctx->state++;
            } else {
                ctx->state = SCALE_BATTLER_SPRITE_STATE_INIT_SCALE;
            }
        }
        break;
    case SCALE_BATTLER_SPRITE_STATE_CLEANUP:
        ov07_02223224(ctx->monSprite);
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }
}

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

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);

void ov07_022245BC(BattleAnimSystem *system);

void ov07_022245BC(BattleAnimSystem *system) {
    BattleAnimSpriteInfo sprites[4];
    int spriteCount;

    ScaleBattlerSpriteContext *ctx = ov07_022324D8(system, sizeof(ScaleBattlerSpriteContext));
    ctx->battleAnimSys = system;

    switch (ov07_0221C4A8(system, 0)) {
    case ((1 << 1) | (1 << 8)):
    case ((1 << 2) | (1 << 8)):
        ctx->battlerSpriteHeight = ov07_0221FAA0(
            ctx->battleAnimSys,
            ov07_0221C468(ctx->battleAnimSys));
        ov07_02232020(
            ctx->battleAnimSys,
            ov07_0221C4A8(system, 0),
            sprites,
            &spriteCount);

        ctx->monSprite = sprites[0].monSprite;
        break;
    case ((1 << 3) | (1 << 8)):
    case ((1 << 4) | (1 << 8)):
        ctx->battlerSpriteHeight = ov07_0221FAA0(
            ctx->battleAnimSys,
            ov07_0221C470(ctx->battleAnimSys));
        ov07_02232020(
            ctx->battleAnimSys,
            ov07_0221C4A8(system, 0),
            sprites,
            &spriteCount);

        ctx->monSprite = sprites[0].monSprite;
        break;
    case ((1 << 11) | (1 << 1)): {
        int battler = 0xFF;

        for (int i = 0; i < 4; i++) {
            int type = ov07_0221FA04(system, i);
            if (type == 0 || type == 2) {
                battler = i;
                break;
            }
        }

        if (battler == 0xFF) {
            battler = 0;
        }

        ctx->monSprite = ov07_0221FA48(ctx->battleAnimSys, battler);
        break;
    }
    case ((1 << 11) | (1 << 2)): {
        int battler = 0xFF;

        for (int i = 0; i < 4; i++) {
            int type = ov07_0221FA04(system, i);
            if (type == 4) {
                battler = i;
                break;
            }
        }

        if (battler == 0xFF) {
            battler = 0;
        }

        ctx->monSprite = ov07_0221FA48(ctx->battleAnimSys, battler);
        break;
    }
    case ((1 << 11) | (1 << 3)): {
        int battler = 0xFF;

        for (int i = 0; i < 4; i++) {
            int type = ov07_0221FA04(system, i);
            if (type == 1 || type == 3) {
                battler = i;
                break;
            }
        }

        if (battler == 0xFF) {
            battler = 0;
        }

        ctx->monSprite = ov07_0221FA48(ctx->battleAnimSys, battler);
        break;
    }
    case ((1 << 11) | (1 << 4)): {
        int battler = 0xFF;

        for (int i = 0; i < 4; i++) {
            int type = ov07_0221FA04(system, i);
            if (type == 5) {
                battler = i;
                break;
            }
        }

        if (battler == 0xFF) {
            battler = 0;
        }

        ctx->monSprite = ov07_0221FA48(ctx->battleAnimSys, battler);
        break;
    }
    }

    if (ctx->monSprite == 0L) {
        Heap_Free(ctx);
        return;
    }

    ctx->battlerY = Pokepic_GetAttr(ctx->monSprite, MON_SPRITE_Y_CENTER);
    ctx->battlerSpriteHeight = Pokepic_GetAttr(ctx->monSprite, MON_SPRITE_SHADOW_HEIGHT);
    ctx->battlerSpriteHeight += 16;

    ctx->startX = ov07_0221C4A8(system, 1);
    ctx->endX = ov07_0221C4A8(system, 2);
    ctx->startY = ov07_0221C4A8(system, 3);
    ctx->endY = ov07_0221C4A8(system, 4);
    ctx->reference = ov07_0221C4A8(system, 5);
    ctx->cycles = ov07_0221C4A8(system, 6);
    ctx->scaleFrames = ov07_0221C4A8(system, 7);
    ctx->cycles = ((ov07_0221C4A8(system, 6)) & 0xFFFF);
    ctx->hold = ((ov07_0221C4A8(system, 6)) >> 16);

    SysTask *task = ov07_0221C410(ctx->battleAnimSys, ov07_0222446C, ctx);
    ov07_0222446C(task, ctx);
}
