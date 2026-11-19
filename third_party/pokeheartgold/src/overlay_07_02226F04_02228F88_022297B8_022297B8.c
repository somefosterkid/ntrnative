#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_74_main_menu_util.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"

struct BattleAnimSystem;

struct ManagedSprite;

struct Pokepic;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

struct TrickCupSprite;

struct TrickSpriteContext;

struct XYTransformContext;

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


void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);

void ManagedSprite_OffsetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

int ov07_0221FAE8(BattleAnimSystem *system);

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

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);

BOOL ov07_022223CC(XYTransformContext *ctx, XYTransformContext *revs, ManagedSprite *sprite);

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

s16 ov07_02222674(s16 baseY, int height, fx32 scaleY);

void ov07_02222DC8(s16 x1, s16 y1, s16 x2, s16 y2, s16 *mx, s16 *my);

void ov07_02222DE4(s32 x1, s32 y1, s32 x2, s32 y2, fx32 *distance);

void ov07_02222E0C(s16 x1, s16 y1, s16 x2, s16 y2, u16 *angle);

BOOL ov07_02222E48(int *value, int target, s32 step);

void Heap_Free(void *ptr);

typedef struct TrickCupSprite {
    ManagedSprite *sprite;
    XYTransformContext arc[2];
} TrickCupSprite;

typedef struct TrickSpriteContext {
    u8 state;
    u8 delay;
    int cupAlpha;
    int bgAlpha;
    u8 iteration;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    Pokepic *attackerSprite;
    s16 attackerY;
    int attackerOffset;
    u16 unused;
    fx32 radius;
    TrickCupSprite cupSprites[2];
    s16 mx;
    s16 my;
    XYTransformContext attackerScale;
} TrickSpriteContext;

enum TrickSpriteState {
    TRICK_SPRITE_STATE_INIT_SCALE = 0,
    TRICK_SPRITE_STATE_SCALE,
    TRICK_SPRITE_STATE_DELAY_1,
    TRICK_SPRITE_STATE_CUP_RISE_1,
    TRICK_SPRITE_STATE_SET_RISE_2,
    TRICK_SPRITE_STATE_CALC_ARC_PARAMS,
    TRICK_SPRITE_STATE_INIT_ARC,
    TRICK_SPRITE_STATE_ARC,
    TRICK_SPRITE_STATE_FADE_OUT,
};

extern const struct {
    u8 sx, ex;
    u8 sy, ey;
    u8 frames;
} ov07_02236704[3];

void ov07_022297B8(SysTask *task, void *param);

typedef struct {
    s8 a[5][2];
} ov07_022366E1_Array;

extern const ov07_022366E1_Array ov07_022366E1;

void ov07_022297B8(SysTask *task, void *param) {
    int i;
    TrickSpriteContext *ctx = param;

    switch (ctx->state) {
    case TRICK_SPRITE_STATE_INIT_SCALE:
        ov07_02222590(
            &ctx->attackerScale,
            ov07_02236704[ctx->iteration].sx,
            ov07_02236704[ctx->iteration].ex,
            ov07_02236704[ctx->iteration].sy,
            ov07_02236704[ctx->iteration].ey,
            100,
            ov07_02236704[ctx->iteration].frames);
        ctx->iteration++;
        ctx->state++;
        break;
    case TRICK_SPRITE_STATE_SCALE:
        if (ov07_0222260C(&ctx->attackerScale) == 1) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_X, ctx->attackerScale.x);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_Y, ctx->attackerScale.y);
            s16 offset = ov07_02222674(
                ctx->attackerY,
                ctx->attackerOffset,
                ctx->attackerScale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY + offset);
        } else {
            if (ctx->iteration >= 3) {
                ctx->state++;
            } else {
                ctx->state--;
            }
        }
        break;
    case TRICK_SPRITE_STATE_DELAY_1:
        if (++ctx->delay >= 1) {
            ctx->delay = 0;
            ctx->iteration = 0;
            ctx->state++;
        }
        break;
    case TRICK_SPRITE_STATE_CUP_RISE_1: {
        s16 posX, posY;
        ManagedSprite_GetPositionXY(ctx->cupSprites[0].sprite, &posX, &posY);

        if (posY <= 54 + 50) {
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[0].sprite, 0, 2);
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[1].sprite, 0, 2);
        } else {
            ctx->state++;
        }

        break;
    }
    case TRICK_SPRITE_STATE_SET_RISE_2: {
        s16 posX, posY;
        ManagedSprite_GetPositionXY(ctx->cupSprites[0].sprite, &posX, &posY);

        if (posY <= 54 + 50) {
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[0].sprite, 0, 2);
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[1].sprite, 0, 2);
        } else {
            G2_ChangeBlendAlpha(16, 0);
            ctx->state++;
        }
        ctx->delay++;

        ov07_02222E48(&ctx->cupAlpha, 16, +16);
        ov07_02222E48(&ctx->bgAlpha, 0, -16);

        G2_ChangeBlendAlpha(ctx->cupAlpha, ctx->bgAlpha);

        if (ctx->delay == 10) {
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[0].sprite, 0, 16);
        }

        if (ctx->delay == 12) {
            ManagedSprite_OffsetPositionXY(ctx->cupSprites[1].sprite, 0, 16);
        }

        break;
    }
    case TRICK_SPRITE_STATE_CALC_ARC_PARAMS:
        if (++ctx->delay < 0 + 1) {
            break;
        }

        ctx->delay = 0;

        s16 cup1X, cup1Y, cup2X, cup2Y;
        ManagedSprite_GetPositionXY(ctx->cupSprites[0].sprite, &cup1X, &cup1Y);
        ManagedSprite_GetPositionXY(ctx->cupSprites[1].sprite, &cup2X, &cup2Y);

        ov07_02222DC8(cup1X, cup1Y, cup2X, cup2Y, &ctx->mx, &ctx->my);
        ov07_02222DE4(cup1X, cup1Y, ctx->mx, ctx->my, &ctx->radius);
        ov07_02222E0C(cup1X, cup1Y, ctx->mx, ctx->my, &ctx->unused);

        ctx->radius = ((fx32)(((40) > 0) ? ((40) * (1 << 12) + 0.5f) : ((40) * (1 << 12) - 0.5f)));
        ctx->state++;
        break;

    case TRICK_SPRITE_STATE_INIT_ARC: {
        s16 cup1X, cup1Y, cup2X, cup2Y;
        ov07_022366E1_Array cupArcDirections = ov07_022366E1;

        ManagedSprite_GetPositionXY(ctx->cupSprites[0].sprite, &cup1X, &cup1Y);
        ManagedSprite_GetPositionXY(ctx->cupSprites[1].sprite, &cup2X, &cup2Y);

        ov07_02222338(
            &ctx->cupSprites[0].arc[0],
            &ctx->cupSprites[0].arc[1],
            cup1X,
            cup2X,
            cup1Y,
            cup2Y,
            10,
            ctx->radius * cupArcDirections.a[ctx->iteration][0]);
        ov07_02222338(
            &ctx->cupSprites[1].arc[0],
            &ctx->cupSprites[1].arc[1],
            cup2X,
            cup1X,
            cup2Y,
            cup1Y,
            10,
            ctx->radius * cupArcDirections.a[ctx->iteration][1]);
    }
        ctx->iteration++;
        ctx->state++;
        break;
    case TRICK_SPRITE_STATE_ARC: {
        u8 switchDone = 0;

        if (ov07_022223CC(
                &ctx->cupSprites[0].arc[0],
                &ctx->cupSprites[0].arc[1],
                ctx->cupSprites[0].sprite)
            == 0) {
            switchDone++;
        }

        if (ov07_022223CC(
                &ctx->cupSprites[1].arc[0],
                &ctx->cupSprites[1].arc[1],
                ctx->cupSprites[1].sprite)
            == 0) {
            switchDone++;
        }

        if (switchDone == 2) {
            if (ctx->iteration == 5) {
                ctx->state++;
            } else {
                ctx->state--;
            }
        }
        break;
    }
    case TRICK_SPRITE_STATE_FADE_OUT: {
        int alphaDone = 0;

        if (ov07_02222E48(&ctx->cupAlpha, 0, -2) == 1) {
            alphaDone++;
        }

        if (ov07_02222E48(&ctx->bgAlpha, 16, +2) == 1) {
            alphaDone++;
        }

        G2_ChangeBlendAlpha(ctx->cupAlpha, ctx->bgAlpha);

        if (alphaDone == 2) {
            ctx->state++;
        }
        break;
    }
    default:
        for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSys, 0); i++) {
            Sprite_DeleteAndFreeResources(ctx->cupSprites[i].sprite);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    s16 x, y;
    for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSys, 0); i++) {
        ManagedSprite_GetPositionXY(ctx->cupSprites[i].sprite, &x, &y);

        if (y < 80) {
            ManagedSprite_SetPriority(
                ctx->cupSprites[i].sprite,
                ov07_0221FAE8(ctx->battleAnimSys));
        } else {
            if (x > 128) {
                ManagedSprite_SetPriority(
                    ctx->cupSprites[i].sprite,
                    ov07_0221FAE8(ctx->battleAnimSys));
            } else {
                ManagedSprite_SetPriority(
                    ctx->cupSprites[i].sprite,
                    ov07_0221FAE8(ctx->battleAnimSys) + 1);
            }
        }
    }

    if (ctx->state < TRICK_SPRITE_STATE_SET_RISE_2) {
        return;
    }

    for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSys, 0); i++) {
        ManagedSprite_TickFrame(ctx->cupSprites[i].sprite);
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
