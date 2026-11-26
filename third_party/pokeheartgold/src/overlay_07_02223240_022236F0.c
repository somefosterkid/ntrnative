#include "global.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "pokepic.h"
#include "sprite.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_funcs.h"

struct BattleAnimSystem;

struct BgConfig;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct BgConfig BgConfig;

void ToggleBgLayer(u8 bgLayer, u8 enable);

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov07_0221C448(BattleAnimSystem *system, SysTask *task);

typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

void Heap_Free(void *ptr);

typedef struct MuddyWaterContext {
    BOOL reverse;
    BOOL scrollFinished;
    u16 unused;
    s16 x;
    s16 y;
    s16 stepX;
    s16 stepY;
    u16 slowDownTime;
    u16 timer;
    u16 bgAlpha;
    u16 otherAlpha;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo shakeInfo;
    BattleAnimSpriteInfo spriteInfo[4];
    XYTransformContext transform;
} MuddyWaterContext;

enum MuddyWaterState {
    MUDDY_WATER_STATE_INIT,
    MUDDY_WATER_STATE_FADE_IN,
    MUDDY_WATER_STATE_WAIT,
    MUDDY_WATER_STATE_FADE_OUT,
    MUDDY_WATER_STATE_END
};

static void ov07_022236F0(SysTask *task, void *param);

static void ov07_022236F0(SysTask *task, void *param) {
    MuddyWaterContext *ctx = (MuddyWaterContext *)param;

    switch (ctx->common.state) {
    case MUDDY_WATER_STATE_INIT:
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3, ctx->bgAlpha, ctx->otherAlpha);
        ToggleBgLayer(GF_BG_LYR_MAIN_2, 1);
        ctx->common.state++;
    case MUDDY_WATER_STATE_FADE_IN: {
        int fadeInComplete = 0;

        if (ctx->bgAlpha < 16) {
            ctx->bgAlpha += 2;
        } else {
            fadeInComplete++;
        }

        if (ctx->otherAlpha > 4) {
            ctx->otherAlpha -= 2;
        } else {
            fadeInComplete++;
        }

        if (fadeInComplete == 2) {
            ctx->bgAlpha = 16;
            ctx->otherAlpha = 4;
            ctx->common.state++;
        }

        G2_ChangeBlendAlpha(ctx->bgAlpha, ctx->otherAlpha);
        break;
    }
    case MUDDY_WATER_STATE_WAIT:
        if (ctx->scrollFinished) {
            ctx->common.state++;
        }
        break;
    case MUDDY_WATER_STATE_FADE_OUT: {
        int fadeOutComplete = 0;

        if (ctx->bgAlpha > 2) {
            ctx->bgAlpha -= 2;
        } else {
            fadeOutComplete++;
        }

        if (ctx->otherAlpha < 16) {
            ctx->otherAlpha += 2;
        } else {
            fadeOutComplete++;
        }

        if (fadeOutComplete == 2) {
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

    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_X, ctx->x);
    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_Y, ctx->y);
}

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

enum BgNarcMemberType {
    BG_NARC_MEMBER_NCGR = 0,
    BG_NARC_MEMBER_NCLR,
    BG_NARC_MEMBER_NSCR1,
    BG_NARC_MEMBER_NSCR2,
    BG_NARC_MEMBER_NSCR3,

    BG_NARC_MEMBER_COUNT,
};

BOOL ov07_0221BFC0(BattleAnimSystem *system);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C468(BattleAnimSystem *system);

s32 ov07_0221C4A8(BattleAnimSystem *system, int index);

int ov07_0221FB7C(int bgID, enum BgNarcMemberType type);

enum Battler {
    BTLSCR_ALL_BATTLERS = 0,
    BTLSCR_ATTACKER = 1,
    BTLSCR_DEFENDER = 2,
    BTLSCR_PLAYER = 3,
    BTLSCR_ENEMY = 4,
    BTLSCR_FAINTED_MON = 5,
    BTLSCR_SWITCHED_MON = 6,
    BTLSCR_SIDE_EFFECT_MON = 7,
    BTLSCR_ABILITY_MON = 8,
    BTLSCR_PLAYER_SLOT_1 = 9,
    BTLSCR_ENEMY_SLOT_1 = 10,
    BTLSCR_PLAYER_SLOT_2 = 11,
    BTLSCR_ENEMY_SLOT_2 = 12,
    BTLSCR_UNUSED_13 = 13,
    BTLSCR_MSG_ATTACKER = 14,
    BTLSCR_MSG_DEFENDER = 15,
    BTLSCR_ATTACKER_PARTNER = 16,
    BTLSCR_DEFENDER_PARTNER = 17,
    BTLSCR_FORCED_OUT = 18,
    BTLSCR_ATTACKER_ENEMY = 19,
    BTLSCR_DEFENDER_ENEMY = 20,
    BTLSCR_MSG_BATTLER_TEMP = 21,
    BTLSCR_SWITCHED_MON_AFTER = 22,
};

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);

u32 GfGfxLoader_LoadCharData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);

void GfGfxLoader_LoadScrnData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);

void ov07_02223864(BattleAnimSystem *system);

void ov07_02223864(BattleAnimSystem *system) {
    MuddyWaterContext *ctx = ov07_022324D8(system, sizeof(MuddyWaterContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->x = ov07_0221C4A8(system, 1);
    ctx->y = ov07_0221C4A8(system, 2);
    ctx->stepX = ov07_0221C4A8(system, 3);
    ctx->stepY = ov07_0221C4A8(system, 4);
    ctx->reverse = ov07_0221C4A8(system, 5);
    ctx->unused = ov07_0221C4A8(system, 6);
    ctx->slowDownTime = ov07_0221C4A8(system, 7);
    ctx->timer = 0;

    if (ctx->reverse != 0 && ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        ctx->x *= -1;
        ctx->y *= -1;
        ctx->stepX *= -1;
        ctx->stepY *= -1;
        ctx->y -= ((128 / 3) * 2);
    } else {
        ctx->y += ((128 / 3) * 2);
    }

    if (ov07_0221BFC0(system) == 1) {
        ctx->stepX *= -1;
    }

    ctx->bgAlpha = 4;
    ctx->otherAlpha = 16;
    ctx->bgAlpha = 0;
    ctx->otherAlpha = 16;
    ctx->scrollFinished = 0;

    ToggleBgLayer(GF_BG_LYR_MAIN_2, 0);

    int bgID = ov07_0221C4A8(system, 0);

    GfGfxLoader_LoadCharData(
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, BG_NARC_MEMBER_NCGR),
        ctx->common.bgConfig,
        GF_BG_LYR_MAIN_2,
        0,
        0,
        1,
        ov07_0221BFD0(system));
    PaletteData_LoadNarc(
        ctx->common.paletteData,
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, BG_NARC_MEMBER_NCLR),
        ov07_0221BFD0(system),
        PLTTBUF_MAIN_BG,
        ((16) * sizeof(u16)),
        ((9) * (16)));
    BgClearTilemapBufferAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2);

    enum BgNarcMemberType member = BG_NARC_MEMBER_NSCR1;

    if (ov07_0221BFC0(system) == 1) {
        member = BG_NARC_MEMBER_NSCR3;
    } else if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        member = BG_NARC_MEMBER_NSCR2;
    }

    GfGfxLoader_LoadScrnData(
        NARC_a_0_0_7,
        ov07_0221FB7C(bgID, member),
        ctx->common.bgConfig,
        GF_BG_LYR_MAIN_2,
        0,
        0,
        1,
        ov07_0221BFD0(system));

    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_X, ctx->x);
    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_OFFSET_UPDATE_SET_Y, ctx->y);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_022236F0, ctx);
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

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

typedef struct RotateMonContext {
    u8 state;
    BattleAnimSystem *system;
    Pokepic *sprite;
    ValueLerpContext lerpCtx;
    s32 start;
    int steps;
    s16 centerX;
    s16 centerY;
} RotateMonContext;

static void ov07_02223A1C(SysTask *task, void *param);

static void ov07_02223A1C(SysTask *task, void *param) {
    RotateMonContext *ctx = param;

    switch (ctx->state) {
    case 0:
    case 1:
        if (ov07_02222440(&ctx->lerpCtx) == TRUE) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_ROTATION_Z, (u16)ctx->lerpCtx.value);
        } else {
            ov07_022223F0(&ctx->lerpCtx, ctx->lerpCtx.value, ctx->start, ctx->steps);
            ctx->state++;
        }
        break;
    case 2:
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_X_PIVOT, 0);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_PIVOT, 0);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_ROTATION_Z, 0);
        ov07_0221C448(ctx->system, task);
        Heap_Free(ctx);
        break;
    }
}

u16 ov07_0221C470(BattleAnimSystem *system);

void ov07_02223A94(BattleAnimSystem *system);

void ov07_02223A94(BattleAnimSystem *system) {
    RotateMonContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(RotateMonContext));

    ctx->state = 0;
    ctx->system = system;
    ctx->steps = ov07_0221C4A8(system, 2);
    ctx->start = ov07_0221C4A8(system, 0);

    int type = ov07_0221C4A8(system, 3);

    int battler;
    if (type == 0 || type == 2) {
        battler = ov07_0221C470(ctx->system);
    } else {
        battler = ov07_0221C468(ctx->system);
    }

    ov07_022223F0(&ctx->lerpCtx, ctx->start, ov07_0221C4A8(system, 1), ctx->steps);

    int dir = ov07_02222004(system, battler);
    ctx->lerpCtx.data[1] *= dir;

    if (type == 2) {
        dir *= -1;
    }

    ctx->sprite = ov07_0221FA48(ctx->system, battler);

    if (ov07_0221C4A8(system, 3) == 1) {
        ctx->centerX = ov07_0221C4A8(system, 4);
        ctx->centerY = ov07_0221C4A8(system, 5);
    } else {
        ctx->centerX = ((10 * 8) / 2) * dir;
        ctx->centerY = ((10 * 8) / 2);
    }

    Pokepic_SetAttr(ctx->sprite, MON_SPRITE_X_PIVOT, ctx->centerX);
    Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_PIVOT, ctx->centerY);

    ov07_0221C410(ctx->system, ov07_02223A1C, ctx);
}

BOOL ov07_02222240(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

typedef struct SubmissionContext {
    BattleAnimSystem *battleAnimSys;
    XYTransformContext rev;
    Pokepic *sprite;
    s16 baseX;
    s16 baseY;
} SubmissionContext;

static void ov07_02223B70(SysTask *task, void *param);

static void ov07_02223B70(SysTask *task, void *param) {
    SubmissionContext *ctx = param;

    if (ov07_02222240(&ctx->rev, ctx->baseX, ctx->baseY, ctx->sprite) == 0) {
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->baseX);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->baseY + (((fx32)(((-8) > 0) ? ((-8) * (1 << 12) + 0.5f) : ((-8) * (1 << 12) - 0.5f))) >> 12));
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_ROTATION_Z, 0);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
    }
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

void ov07_02222A44(XYTransformContext *ctx, int revs, int stepsPerRev);

int ov07_0223197C(BattleAnimSystem *system, int battler);

void ov07_02223BC0(BattleAnimSystem *system);

void ov07_02223BC0(BattleAnimSystem *system) {
    SubmissionContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(SubmissionContext));
    ctx->battleAnimSys = system;

    ov07_02222A44(
        &ctx->rev,
        ov07_0221C4A8(system, 0),
        ov07_0221C4A8(system, 1));

    switch (ov07_0221C4A8(system, 2)) {
    case (1 << 1):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
        ctx->rev.data[XY_PARAM_REV_RADIUS_X] *= -1;
        break;
    case (1 << 2):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0223197C(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys)));
        ctx->rev.data[XY_PARAM_REV_RADIUS_X] *= -1;
        break;
    case (1 << 3):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
        break;
    case (1 << 4):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0223197C(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)));
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    ctx->baseX = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_X_CENTER);
    ctx->baseY = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_Y_CENTER);
    ctx->baseY -= ((fx32)(((-8) > 0) ? ((-8) * (1 << 12) + 0.5f) : ((-8) * (1 << 12) - 0.5f))) >> 12;

    ov07_0221C410(ctx->battleAnimSys, ov07_02223B70, ctx);
}
