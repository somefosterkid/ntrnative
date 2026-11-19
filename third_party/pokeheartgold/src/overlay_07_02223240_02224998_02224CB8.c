#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "gf_gfx_planes.h"
#include "pokepic.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

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

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

s16 ov07_02222674(s16 baseY, int height, fx32 scaleY);

BOOL ov07_022227D8(XYTransformContext *ctx);

typedef struct ShakeAndScaleAttackerContext {
    u8 state;
    u8 unused0;
    s16 spriteY;
    int spriteHeight;
    u8 sLotoId;
    u8 timer;
    u8 holdFrames;
    BattleAnimSystem *battleAnimSys;
    Pokepic *sprite;
    XYTransformContext scale;
    XYTransformContext shake;
} ShakeAndScaleAttackerContext;

enum ShakeAndScaleAttackerState {
    SHAKE_AND_SCALE_ATTACKER_STATE_SCALE_1 = 0,
    SHAKE_AND_SCALE_ATTACKER_STATE_HOLD,
    SHAKE_AND_SCALE_ATTACKER_STATE_SCALE_2,
    SHAKE_AND_SCALE_ATTACKER_STATE_WAIT,
};

static void ov07_02224CB8(SysTask *task, void *param);

static void ov07_02224CB8(SysTask *task, void *param) {
    ShakeAndScaleAttackerContext *ctx = param;

    switch (ctx->state) {
    case SHAKE_AND_SCALE_ATTACKER_STATE_SCALE_1:
        if (ov07_02222558(&ctx->scale) == 1) {
            ov07_022227D8(&ctx->shake);

            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            Pokepic_AddAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->shake.x);

            s16 offset = ov07_02222674(ctx->spriteY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->spriteY + offset);
        } else {
            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_STATE_HOLD:
        if (++ctx->timer > ctx->holdFrames) {
            ov07_02222508(
                &ctx->scale,
                (s16)ov07_0221C4A8(ctx->battleAnimSys, 0),
                100,
                (s16)ov07_0221C4A8(ctx->battleAnimSys, 1),
                ov07_0221C4A8(ctx->battleAnimSys, 3));
            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_STATE_SCALE_2:
        if (ov07_02222558(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            s16 offset = ov07_02222674(ctx->spriteY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->spriteY + offset);
        } else {
            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_STATE_WAIT:
        if (ov07_02222558(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);

            s16 offset = ov07_02222674(ctx->spriteY, ctx->spriteHeight, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->spriteY + offset);
        } else {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->spriteY);
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

void ov07_02224E58(BattleAnimSystem *system);

void ov07_02224E58(BattleAnimSystem *system) {
    ShakeAndScaleAttackerContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(ShakeAndScaleAttackerContext));

    ctx->sLotoId = 0;
    ctx->state = SHAKE_AND_SCALE_ATTACKER_STATE_SCALE_1;
    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->timer = 0;
    ctx->holdFrames = ov07_0221C4A8(system, 4);
    ctx->spriteY = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_Y_CENTER);
    ctx->spriteHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ov07_02222508(
        &ctx->scale,
        100,
        100,
        (s16)ov07_0221C4A8(system, 0),
        ov07_0221C4A8(system, 2));

    ov07_022227A8(
        &ctx->shake,
        2,
        0,
        0,
        10);

    ov07_0221C410(ctx->battleAnimSys, ov07_02224CB8, ctx);
}

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

typedef struct ShakeAndScaleAttacker2Context {
    u8 state;
    u8 unused0;
    s16 posY;
    int spriteOffset;
    u8 sLotoId;
    u8 timer;
    u8 holdFrames;
    int scale2X;
    int scale2Y;
    int scale2Frames;
    BattleAnimSystem *battleAnimSys;
    Pokepic *sprite;
    XYTransformContext scale;
    XYTransformContext shake;
} ShakeAndScaleAttacker2Context;

enum ShakeAndScaleAttacker2State {
    SHAKE_AND_SCALE_ATTACKER_2_STATE_SCALE_1 = 0,
    SHAKE_AND_SCALE_ATTACKER_2_STATE_HOLD,
    SHAKE_AND_SCALE_ATTACKER_2_STATE_SCALE_2,
    SHAKE_AND_SCALE_ATTACKER_2_STATE_WAIT,
};

static void ov07_02224EF4(SysTask *task, void *param);

static void ov07_02224EF4(SysTask *task, void *param) {
    ShakeAndScaleAttacker2Context *ctx = param;

    switch (ctx->state) {
    case SHAKE_AND_SCALE_ATTACKER_2_STATE_SCALE_1:
        if (ov07_0222260C(&ctx->scale) == 1) {
            ov07_022227D8(&ctx->shake);

            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);
            Pokepic_AddAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->shake.x);

            s16 offset = ov07_02222674(ctx->posY, ctx->spriteOffset, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->posY + offset);
        } else {
            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_2_STATE_HOLD:
        if (++ctx->timer > ctx->holdFrames) {
            s16 sx = ((ctx->scale2X) >> 16);
            s16 ex = ((ctx->scale2X) & 0xFF);
            s16 sy = ((ctx->scale2Y) >> 16);
            s16 ey = ((ctx->scale2Y) & 0xFF);

            ov07_02222590(&ctx->scale, sx, ex, sy, ey, 100, ctx->scale2Frames);

            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_2_STATE_SCALE_2:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);

            s16 offset = ov07_02222674(ctx->posY, ctx->spriteOffset, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->posY + offset);
        } else {
            ctx->state++;
        }
        break;
    case SHAKE_AND_SCALE_ATTACKER_2_STATE_WAIT:
        if (ov07_0222260C(&ctx->scale) == 1) {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, ctx->scale.x);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, ctx->scale.y);

            s16 offset = ov07_02222674(ctx->posY, ctx->spriteOffset, ctx->scale.data[XY_PARAM_CUR_Y]);
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->posY + offset);
        } else {
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, (1 << 8));
            Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->posY);
            ctx->state++;
        }
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_02221F38(BattleAnimSystem *system, int battler, s16 *px, s16 *py);

void ov07_02225098(BattleAnimSystem *system);

void ov07_02225098(BattleAnimSystem *system) {
    ShakeAndScaleAttacker2Context *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(ShakeAndScaleAttacker2Context));

    ctx->sLotoId = 0;
    ctx->state = SHAKE_AND_SCALE_ATTACKER_2_STATE_SCALE_1;
    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->timer = 0;
    ctx->holdFrames = ov07_0221C4A8(system, 6);
    ctx->scale2X = ov07_0221C4A8(ctx->battleAnimSys, 3);
    ctx->scale2Y = ov07_0221C4A8(ctx->battleAnimSys, 4);
    ctx->scale2Frames = ov07_0221C4A8(ctx->battleAnimSys, 5);

    ov07_02221F38(system, ov07_0221C468(ctx->battleAnimSys), 0L, &ctx->posY);

    ctx->spriteOffset = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->posY += ctx->spriteOffset;

    s16 sx, sy, ex, ey;
    sx = ((ov07_0221C4A8(system, 0)) >> 16);
    ex = ((ov07_0221C4A8(system, 0)) & 0xFF);
    sy = ((ov07_0221C4A8(system, 1)) >> 16);
    ey = ((ov07_0221C4A8(system, 1)) & 0xFF);

    ov07_02222590(
        &ctx->scale,
        sx,
        ex,
        sy,
        ey,
        100,
        ov07_0221C4A8(system, 2));
    ov07_022227A8(
        &ctx->shake,
        2,
        0,
        0,
        10);

    ov07_0221C410(ctx->battleAnimSys, ov07_02224EF4, ctx);
}

void ov07_0221EC7C(BattleAnimSystem *system, enum GFBgLayer bgLayer);

typedef struct SuperpowerContext {
    int unused0;
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sLotoId;
    ManagedSprite *sprite;
} SuperpowerContext;

static void ov07_0222519C(SysTask *task, void *param);

static void ov07_0222519C(SysTask *task, void *param) {
    SuperpowerContext *ctx = param;

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    ov07_0221EC7C(ctx->common.battleAnimSys, GF_BG_LYR_MAIN_2);
    ov07_0221C448(ctx->common.battleAnimSys, task);
    Heap_Free(ctx);
}

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

void ov07_0221EB98(BattleAnimSystem *system, enum GFBgLayer bgLayer);

void ov07_022251CC(BattleAnimSystem *system);

void ov07_022251CC(BattleAnimSystem *system) {
    SuperpowerContext *ctx = ov07_022324D8(system, sizeof(SuperpowerContext));
    (void)(ov07_0221C4A8(system, 0));

    ov07_02231FE4(system, &ctx->common);
    ov07_0221EB98(ctx->common.battleAnimSys, GF_BG_LYR_MAIN_2);

    ctx->sprite = ov07_0221C4E8(ctx->common.battleAnimSys, 0);

    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_OBJWND);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetAffineScale(ctx->sprite, 1.2f, 1.2f);

    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 0);
    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2, 0);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222519C, ctx);
}

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222B4(XYTransformContext *ctx);

typedef struct MoveBattlerContext {
    int unk_00;
    int unk_04;
    int frames;
    BattleAnimSpriteInfo spriteInfo;
    BattleAnimScriptFuncCommon common;
    XYTransformContext pos;
    BattleAnimSystem *battleAnimSys;
    s16 offsetX;
    s16 offsetY;
} MoveBattlerContext;

enum MoveBattlerState {
    MOVE_BATTLER_STATE_INIT = 0,
    MOVE_BATTLER_STATE_MOVE,
};

void ov07_0222525C(SysTask *task, void *param);

void ov07_0222525C(SysTask *task, void *param) {
    MoveBattlerContext *ctx = param;

    switch (ctx->common.state) {
    case MOVE_BATTLER_STATE_INIT:
        ov07_02222268(
            &ctx->pos,
            ctx->spriteInfo.pos.x,
            ctx->spriteInfo.pos.x + ctx->offsetX,
            ctx->spriteInfo.pos.y,
            ctx->spriteInfo.pos.y + ctx->offsetY,
            ctx->frames);
        ctx->common.state++;
    case MOVE_BATTLER_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos) == FALSE) {
            ctx->common.state++;
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER, ctx->pos.x);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->pos.y);
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
