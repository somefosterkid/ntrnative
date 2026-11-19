#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "pokepic.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_shake_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);

void ov07_022323FC(BattleAnimSystem *system, int pokemon, BattleAnimSpriteInfo *sprites, int *count);

static inline BOOL BattleAnimUtil_IsMaskSet(int value, int mask) {
    if ((value & mask) == mask) {
        return 1;
    }

    return 0;
}

void ov07_02223350(BattleAnimSystem *system);

void ov07_02223350(BattleAnimSystem *system) {
    ShakeContext *ctx = ov07_022324D8(system, sizeof(ShakeContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->shakeInfo.extentX = ov07_0221C4A8(system, 0);
    ctx->shakeInfo.extentY = ov07_0221C4A8(system, 1);
    ctx->shakeInfo.interval = ov07_0221C4A8(system, 2);
    ctx->shakeInfo.amount = ov07_0221C4A8(system, 3);
    ctx->shakeInfo.targets = ov07_0221C4A8(system, 4);

    if (BattleAnimUtil_IsMaskSet(ctx->shakeInfo.targets, (1 << 8)) == 1) {
        ov07_02232020(system, ctx->shakeInfo.targets, &ctx->sprites[0], &ctx->spriteCount);
    } else if (BattleAnimUtil_IsMaskSet(ctx->shakeInfo.targets, (1 << 9)) == 1) {
        ov07_022323FC(system, ctx->shakeInfo.targets, &ctx->sprites[0], &ctx->spriteCount);
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02223240, ctx);
}

void ToggleBgLayer(u8 bgLayer, u8 enable);

typedef struct ScrollCustomBgContext {
    BOOL reverse;
    BOOL scrollFinished;
    u16 targetBgAlpha;
    s16 x;
    s16 y;
    s16 stepX;
    s16 stepY;
    u16 slowDownTime;
    u16 timer;
    u16 bgAlpha;
    u16 otherAlpha;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo unused0;
    BattleAnimSpriteInfo sLotoId[4];
    XYTransformContext unused2;
} ScrollCustomBgContext;

enum ScrollCustomBgState {
    SCROLL_CUSTOM_BG_STATE_INIT = 0,
    SCROLL_CUSTOM_BG_STATE_FADE_IN,
    SCROLL_CUSTOM_BG_STATE_WAIT,
    SCROLL_CUSTOM_BG_STATE_FADE_OUT,
};

void ov07_022233D8(SysTask *task, void *param);

void ov07_022233D8(SysTask *task, void *param) {
    ScrollCustomBgContext *ctx = param;

    switch (ctx->common.state) {
    case SCROLL_CUSTOM_BG_STATE_INIT:
        G2_SetBlendAlpha(
            GX_BLEND_PLANEMASK_BG2,
            GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3,
            ctx->bgAlpha,
            ctx->otherAlpha);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, 1);
        ctx->common.state++;
    case SCROLL_CUSTOM_BG_STATE_FADE_IN: {
        int completed = 0;

        if (ctx->bgAlpha < ctx->targetBgAlpha - 2) {
            ctx->bgAlpha += 2;
        } else {
            completed++;
        }

        if (ctx->otherAlpha > 7 + 2) {
            ctx->otherAlpha -= 2;
        } else {
            completed++;
        }

        if (completed == 2) {
            ctx->bgAlpha = ctx->targetBgAlpha;
            ctx->otherAlpha = 7;
            ctx->common.state++;
        }

        G2_ChangeBlendAlpha(ctx->bgAlpha, ctx->otherAlpha);
        break;
    }
    case SCROLL_CUSTOM_BG_STATE_WAIT:
        if (ctx->scrollFinished) {
            ctx->common.state++;
        }
        break;
    case SCROLL_CUSTOM_BG_STATE_FADE_OUT: {
        int completed = 0;

        if (ctx->bgAlpha > 2) {
            ctx->bgAlpha -= 2;
        } else {
            completed++;
        }

        if (ctx->otherAlpha < 31 - 2) {
            ctx->otherAlpha += 2;
        } else {
            completed++;
        }

        if (completed == 2) {
            ctx->bgAlpha = 0;
            ctx->otherAlpha = 31;
            ctx->common.state++;
        }

        G2_ChangeBlendAlpha(ctx->bgAlpha, ctx->otherAlpha);
        break;
    }
    default:
        ToggleBgLayer(GF_BG_LYR_MAIN_2, 0);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    if ((ctx->stepY > 0 && ctx->y >= 512) || (ctx->stepY < 0 && ctx->y <= (-412))) {
        ctx->scrollFinished = 1;
    }

    ctx->x += ctx->stepX;
    ctx->y += ctx->stepY;

    if (ctx->slowDownTime < ctx->timer) {
        if (ctx->stepY < 0) {
            ctx->stepY++;
        } else {
            ctx->stepY = 0;
        }

        if (ctx->stepX < 0) {
            ctx->stepX++;
        } else {
            ctx->stepX = 0;
        }

        ctx->timer = 0;
    } else {
        ctx->timer++;
    }

    BgSetPosTextAndCommit(ctx->common.bgConfig, 2, BG_POS_OP_SET_X, ctx->x);
    BgSetPosTextAndCommit(ctx->common.bgConfig, 2, BG_POS_OP_SET_Y, ctx->y);
}
