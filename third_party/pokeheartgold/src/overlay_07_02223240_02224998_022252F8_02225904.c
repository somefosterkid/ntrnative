#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_shake_bg_context.h"

struct BattleAnimSystem;

struct BgConfig;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

typedef struct BgConfig BgConfig;

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct BattlerPartialDrawTestContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    int xOffset;
    int yOffset;
    int width;
    int height;
    s16 unused;
    s16 step;
    int stepFrames;
    int mode;
    int timer;
    int y;
    int drawHeight;
} BattlerPartialDrawTestContext;

void ov07_02223224(Pokepic *sprite);

static void ov07_02225904(SysTask *task, void *param);

static void ov07_02225904(SysTask *task, void *param) {
    BattlerPartialDrawTestContext *ctx = param;

    switch (ctx->common.state) {
    case 0: {
        if (++ctx->timer < ctx->stepFrames) {
            return;
        }

        ctx->timer = 0;

        if (ctx->mode == 0) {
            if (ctx->drawHeight < 0) {
                ctx->drawHeight = 0;
            }

            if (ctx->drawHeight == 0) {
                ctx->common.state++;
            }
        } else {
            if (ctx->drawHeight > (10 * 8)) {
                ctx->drawHeight = (10 * 8);
            }

            if (ctx->drawHeight == (10 * 8)) {
                ctx->common.state++;
            }
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT, ctx->drawHeight);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->y);

        ctx->y -= ctx->step;
        ctx->drawHeight += ctx->step;
        break;
    }
    default:
        ov07_02223224(ctx->spriteInfo.monSprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void Pokepic_SetVisible(Pokepic *monSprite, int xOffset, int yOffset, int width, int height);

int ov07_0223197C(BattleAnimSystem *system, int battler);

void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);

void ov07_02225990(BattleAnimSystem *system);

void ov07_02225990(BattleAnimSystem *system) {
    BattlerPartialDrawTestContext *ctx = ov07_022324D8(system, sizeof(BattlerPartialDrawTestContext));
    ov07_02231FE4(system, &ctx->common);

    int target = ov07_0221C4A8(system, 0);

    int count;
    ov07_02232020(system, target, &ctx->spriteInfo, &count);

    int battler;
    switch (target) {
    case (1 << 1):
        battler = ov07_0221C468(system);
        break;
    case (1 << 2):
        battler = ov07_0223197C(system, ov07_0221C468(system));
        break;
    case (1 << 3):
        battler = ov07_0221C470(system);
        break;
    case (1 << 4):
        battler = ov07_0223197C(system, ov07_0221C470(system));
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    ctx->mode = ov07_0221C4A8(system, 1);
    if (ctx->mode == 0) {
        ctx->y = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER);
        ctx->drawHeight = (10 * 8) - Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT);
        ctx->unused = ov07_0221C4A8(system, 2);
        ctx->step = ov07_0221C4A8(system, 3);
        ctx->step *= -1;
    } else {
        ctx->y = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER);
        ctx->drawHeight = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT);
        ctx->unused = ov07_0221C4A8(system, 2);
        ctx->step = ov07_0221C4A8(system, 3);
    }

    ctx->xOffset = 0;
    ctx->yOffset = 0;
    ctx->width = (10 * 8);
    ctx->height = (10 * 8) - ov07_0221FAA0(system, battler);
    ctx->stepFrames = ov07_0221C4A8(system, 4);
    ctx->timer = 0;

    Pokepic_SetVisible(ctx->spriteInfo.monSprite, ctx->xOffset, ctx->yOffset, ctx->width, ctx->height);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02225904, ctx);
}

static int ov07_02225AE0(BattleAnimSystem *system, int battlerFlags);

static int ov07_02225AE0(BattleAnimSystem *system, int battlerFlags) {
    int battler = 0;

    switch (battlerFlags) {
    case (1 << 1):
        battler = ov07_0221C468(system);
        break;
    case (1 << 2):
        battler = ov07_0223197C(system, ov07_0221C468(system));
        break;
    case (1 << 3):
        battler = ov07_0221C470(system);
        break;
    case (1 << 4):
        battler = ov07_0223197C(system, ov07_0221C470(system));
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return battler;
}

typedef struct BattlerPartialDrawContext {
    BattleAnimScriptFuncCommon common;
    s16 step;
    int stepInterval;
    int timer;
    BattleAnimSpriteInfo spriteInfo;
    int yOffset;
    int height;
    int y;
    int baseY;
    int unk_4C;
} BattlerPartialDrawContext;

enum BattlerPartialDrawState {
    BATTLE_PARTIAL_DRAW_STATE_DRAW = 0,
    BATTLE_PARTIAL_DRAW_STATE_CLEANUP,
};

static void ov07_02225B38(SysTask *task, void *param);

static void ov07_02225B38(SysTask *task, void *param) {
    BattlerPartialDrawContext *ctx = param;

    switch (ctx->common.state) {
    case BATTLE_PARTIAL_DRAW_STATE_DRAW:
        if (++ctx->timer < ctx->stepInterval) {
            break;
        }

        ctx->timer = 0;
        ctx->y += ctx->step;
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->y);
        ctx->yOffset -= ctx->step;

        if (ctx->yOffset > ctx->height) {
            ctx->yOffset = ctx->height;
            ctx->common.state++;
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT, ctx->yOffset);
        break;
    case BATTLE_PARTIAL_DRAW_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_PARTIAL_DRAW, FALSE);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->baseY);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT, ctx->height);
        ov07_02223224(ctx->spriteInfo.monSprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

static void ov07_02225BC4(SysTask *task, void *param);

static void ov07_02225BC4(SysTask *task, void *param) {
    BattlerPartialDrawContext *ctx = param;

    switch (ctx->common.state) {
    case BATTLE_PARTIAL_DRAW_STATE_DRAW:
        if (++ctx->timer < ctx->stepInterval) {
            break;
        }

        ctx->timer = 0;
        ctx->y += ctx->step;
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->y);
        ctx->yOffset -= ctx->step;

        if (ctx->yOffset < 0) {
            ctx->yOffset = 0;
            ctx->common.state++;
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT, ctx->yOffset);
        break;
    case BATTLE_PARTIAL_DRAW_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_PARTIAL_DRAW, FALSE);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->baseY);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_DRAW_HEIGHT, ctx->height);
        ov07_02223224(ctx->spriteInfo.monSprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

static void ov07_02225C50(SysTask *task, void *param);

static void ov07_02225C50(SysTask *task, void *param) {
    BattlerPartialDrawContext *ctx = param;

    switch (ctx->common.state) {
    case BATTLE_PARTIAL_DRAW_STATE_DRAW:
        if (++ctx->timer < ctx->stepInterval) {
            break;
        }

        ctx->timer = 0;
        ctx->yOffset -= ctx->step;
        ctx->height += ctx->step;

        if (ctx->yOffset < 0) {
            ctx->yOffset = 0;
            ctx->common.state++;
        }

        Pokepic_SetVisible(ctx->spriteInfo.monSprite, 0, ctx->yOffset, (10 * 8), ctx->height);
        break;
    case BATTLE_PARTIAL_DRAW_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_PARTIAL_DRAW, 0);
        ov07_02223224(ctx->spriteInfo.monSprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_02225CC4(BattleAnimSystem *system);

void ov07_02225CC4(BattleAnimSystem *system) {
    BattlerPartialDrawContext *ctx = ov07_022324D8(system, sizeof(BattlerPartialDrawContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->step = ov07_0221C4A8(system, 3);
    ctx->stepInterval = ov07_0221C4A8(system, 4);

    int target = ov07_0221C4A8(system, 0);

    int count;
    ov07_02232020(system, target, &ctx->spriteInfo, &count);
    int battler = ov07_02225AE0(system, target);

    if (ov07_0221C4A8(system, 5) != 0) {
        ctx->yOffset = ov07_0221FAA0(system, battler);
        ctx->yOffset = (10 * 8) - ctx->yOffset;
        ctx->height = 0;

        Pokepic_SetVisible(ctx->spriteInfo.monSprite, 0, ctx->yOffset, (10 * 8), ctx->height);
        ov07_0221C410(ctx->common.battleAnimSys, ov07_02225C50, ctx);

        return;
    }

    ctx->y = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER);
    ctx->baseY = ctx->y;
    ctx->yOffset = ov07_0221FAA0(system, battler);
    ctx->yOffset = (10 * 8) - ctx->yOffset;
    ctx->height = ctx->yOffset;

    if (ctx->step > 0) {
        Pokepic_SetVisible(ctx->spriteInfo.monSprite, 0, 0, (10 * 8), ctx->yOffset);
        ov07_0221C410(ctx->common.battleAnimSys, ov07_02225BC4, ctx);
    } else {
        ctx->yOffset = 0;
        ctx->y += ctx->height;
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->y);
        Pokepic_SetVisible(ctx->spriteInfo.monSprite, 0, 0, (10 * 8), ctx->yOffset);
        ov07_0221C410(ctx->common.battleAnimSys, ov07_02225B38, ctx);
    }
}

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_022227D8(XYTransformContext *ctx);

enum ShakeBgState {
    SHAKE_BG_STATE_INIT = 0,
    SHAKE_BG_STATE_SHAKE,
};

void ov07_02225DC4(SysTask *task, void *param);

void ov07_02225DC4(SysTask *task, void *param) {
    ShakeBgContext *ctx = param;

    switch (ctx->state) {
    case SHAKE_BG_STATE_INIT:
        ov07_022227A8(&ctx->shake, ctx->extentX, ctx->extentY, ctx->interval, ctx->amount);
        ctx->state++;
    case SHAKE_BG_STATE_SHAKE: {
        if (ov07_022227D8(&ctx->shake) == FALSE) {
            if (ctx->iteration >= ctx->cycles) {
                ctx->state++;
            } else {
                ctx->state--;
            }

            if (ctx->resetX != FALSE) {
                BgSetPosTextAndCommit(ctx->bgConfig, ctx->bgLayer, BG_OFFSET_UPDATE_SET_X, 0);
            }

            if (ctx->resetY != FALSE) {
                BgSetPosTextAndCommit(ctx->bgConfig, ctx->bgLayer, BG_OFFSET_UPDATE_SET_Y, 0);
            }

            ctx->iteration++;
        } else {
            if (ctx->shake.x != 0) {
                ctx->resetX = TRUE;
                BgSetPosTextAndCommit(ctx->bgConfig, ctx->bgLayer, BG_OFFSET_UPDATE_SET_X, ctx->shake.x);
            }

            if (ctx->shake.y != 0) {
                ctx->resetY = TRUE;
                BgSetPosTextAndCommit(ctx->bgConfig, ctx->bgLayer, BG_OFFSET_UPDATE_SET_Y, ctx->shake.y);
            }
        }
        break;
    }
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
