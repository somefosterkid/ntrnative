#include "global.h"

#include "bg_window.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

BOOL ov07_022223CC(XYTransformContext *ctx, XYTransformContext *revs, ManagedSprite *sprite);

BOOL ov07_02222440(ValueLerpContext *ctx);

void ov07_02232508(void *ptr);

typedef struct IcicleSpearSpriteContext {
    int sLotoId;
    int movementFrames;
    int arcRadius;
    Point2D targetOffset;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo unused2;
    ManagedSprite *sprite;
    BattleAnimSpriteInfo spriteInfo[4];
    XYTransformContext pos[2];
    ValueLerpContext angle;
} IcicleSpearSpriteContext;

static void ov07_0222928C(SysTask *task, void *param);

static void ov07_0222928C(SysTask *task, void *param) {
    IcicleSpearSpriteContext *ctx = param;

    if (ov07_022223CC(&ctx->pos[0], &ctx->pos[1], ctx->sprite) == 0) {
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        ctx = 0L;
        return;
    }

    if (ov07_02222440(&ctx->angle) == 1) {
        ManagedSprite_SetAffineZRotation(ctx->sprite, ctx->angle.value);
    }

    ManagedSprite_TickFrame(ctx->sprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

void ov07_02231FA0(Pokepic *sprite, Point2D *pos);

void ov07_022292E4(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022292E4(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    IcicleSpearSpriteContext *ctx = ov07_022324D8(system, sizeof(IcicleSpearSpriteContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->targetOffset.x = ov07_0221C4A8(system, 0);
    ctx->targetOffset.y = ov07_0221C4A8(system, 1);
    ctx->movementFrames = ov07_0221C4A8(system, 2);
    ctx->arcRadius = ov07_0221C4A8(system, 3);

    int dir = ov07_02222004(system, ov07_0221C468(system));
    ctx->sprite = sprite;

    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    ctx->spriteInfo[0].monSprite = ov07_0221FA48(system, ov07_0221C468(system));
    ctx->spriteInfo[1].monSprite = ov07_0221FA48(system, ov07_0221C470(system));

    ov07_02231FA0(ctx->spriteInfo[0].monSprite, &ctx->spriteInfo[0].pos);
    ov07_02231FA0(ctx->spriteInfo[1].monSprite, &ctx->spriteInfo[1].pos);
    ov07_02222338(&ctx->pos[0],
        &ctx->pos[1],
        ctx->spriteInfo[0].pos.x,
        ctx->spriteInfo[1].pos.x + (ctx->targetOffset.x * dir),
        ctx->spriteInfo[0].pos.y,
        ctx->spriteInfo[1].pos.y + (ctx->targetOffset.y * dir),
        ctx->movementFrames,
        ctx->arcRadius * ((fx32)0x0000000000001000L));

    if (ov07_0221FA04(system, ov07_0221C468(system)) == 4
        && ov07_0221FA04(system, ov07_0221C470(system)) == 2) {
        dir *= -1;
    }

    if (ov07_0221FA04(system, ov07_0221C468(system)) == 5
        && ov07_0221FA04(system, ov07_0221C470(system)) == 3) {
        dir *= -1;
    }

    if (dir > 0) {
        ov07_022223F0(&ctx->angle,
            (((20) * 0xFFFF) / 360) * dir,
            (((130) * 0xFFFF) / 360) * dir,
            10);
    } else {
        ov07_022223F0(&ctx->angle,
            (((90) * 0xFFFF) / 360) * dir,
            (((130) * 0xFFFF) / 360) * dir,
            10);
    }

    ManagedSprite_SetAffineZRotation(ctx->sprite, ctx->angle.value);
    ov07_022223CC(&ctx->pos[0], &ctx->pos[1], ctx->sprite);

    ManagedSprite_TickFrame(ctx->sprite);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222928C, ctx);
}

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

BOOL ov07_0222260C(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

s16 ov07_02222674(s16 baseY, int height, fx32 scaleY);

typedef struct SurfContext {
    int state;
    int holdTimer;
    int unused0;
    u8 alphaStart;
    u8 alphaBg;
    u8 alphaEnd;
    u8 alphaReset;
    u8 alphaTargetStart;
    u8 alphaTargetBg;
    s16 dir;
    Point2D spritePos;
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo sLotoId;
    ManagedSprite *currentSprite;
    ManagedSprite *sprites[2];
    BattleAnimSpriteInfo unused2[4];
    XYTransformContext scale;
    ValueLerpContext unused3;
} SurfContext;

enum SurfState {
    SURF_STATE_COMPRESS = 0,
    SURF_STATE_STRETCH,
    SURF_STATE_HOLD,
    SURF_STATE_RESTORE,
};

static void ov07_02229480(SysTask *task, void *param);

static void ov07_02229480(SysTask *task, void *param) {
    SurfContext *ctx = param;

    switch (ctx->common.state) {
    case SURF_STATE_COMPRESS:
        ov07_02222590(
            &ctx->scale,
            100 * ctx->dir,
            60 * ctx->dir,
            5,
            150,
            100,
            12);

        ctx->common.state++;
        break;
    case SURF_STATE_STRETCH:
        if (ov07_0222260C(&ctx->scale) == 0) {
            ov07_02222590(&ctx->scale, 60 * ctx->dir, 150 * ctx->dir, 150, 10, 100, 12);
            ctx->common.state++;
        } else {
            f32 scaleX, scaleY;
            ov07_02222644(&ctx->scale, &scaleX, &scaleY);
            ManagedSprite_SetAffineScale(ctx->currentSprite, scaleX, scaleY);
            s16 offsetY = ov07_02222674(ctx->spritePos.y, 16, ctx->scale.data[4]);
            ManagedSprite_SetPositionXY(ctx->currentSprite, ctx->spritePos.x, ctx->spritePos.y + offsetY);

            if (ctx->alphaStart < ctx->alphaTargetStart) {
                ctx->alphaStart++;
            }

            if (ctx->alphaBg > ctx->alphaTargetBg) {
                ctx->alphaBg--;
            }

            G2_ChangeBlendAlpha(ctx->alphaStart, ctx->alphaBg);
        }
        break;
    case SURF_STATE_HOLD:
        if (++ctx->holdTimer >= 4) {
            ctx->common.state++;
        }
        break;
    case SURF_STATE_RESTORE:
        if (ov07_0222260C(&ctx->scale) == 0) {
            ctx->common.state++;
        } else {
            f32 scaleX, scaleY;
            ov07_02222644(&ctx->scale, &scaleX, &scaleY);
            ManagedSprite_SetAffineScale(ctx->currentSprite, scaleX, scaleY);
            s16 offsetY = ov07_02222674(ctx->spritePos.y, 16, ctx->scale.data[4]);
            ManagedSprite_SetPositionXY(ctx->currentSprite, ctx->spritePos.x, ctx->spritePos.y + offsetY);

            if (ctx->alphaStart > ctx->alphaEnd) {
                ctx->alphaStart--;
            }

            if (ctx->alphaBg < ctx->alphaReset) {
                ctx->alphaBg++;
            }

            G2_ChangeBlendAlpha(ctx->alphaStart, ctx->alphaBg);
        }
        break;
    default:
        Sprite_DeleteAndFreeResources(ctx->sprites[0]);
        Sprite_DeleteAndFreeResources(ctx->sprites[1]);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        (ctx) = 0L;
        return;
    }

    ManagedSprite_TickFrame(ctx->currentSprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

BOOL ov07_0221BFC0(BattleAnimSystem *system);

ManagedSprite *ov07_0221C4C0(BattleAnimSystem *system, int index);

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

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_02229630(BattleAnimSystem *system);

void ov07_02229630(BattleAnimSystem *system) {
    SurfContext *ctx = ov07_022324D8(system, sizeof(SurfContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->sprites[0] = ov07_0221C4C0(system, 0);
    ctx->sprites[1] = ov07_0221C4C0(system, 1);

    ov07_02231E08(ctx->common.battleAnimSys, -1, -1);

    if (ov07_0221C4A8(system, 0) == 0) {
        ctx->alphaStart = 0;
        ctx->alphaBg = 31;
        ctx->alphaEnd = 0;
        ctx->alphaReset = 31;
        ctx->alphaTargetStart = 31;
        ctx->alphaTargetBg = 4;
    } else {
        ctx->alphaStart = 0;
        ctx->alphaBg = 31;
        ctx->alphaEnd = 0;
        ctx->alphaReset = 31;
        ctx->alphaTargetStart = 31;
        ctx->alphaTargetBg = 4;
    }

    if (ov07_0221BFC0(system) == 1) {
        ctx->currentSprite = ctx->sprites[0];
        ManagedSprite_SetDrawFlag(ctx->sprites[1], 0);
        ManagedSprite_SetAnim(ctx->currentSprite, 0);
        ctx->spritePos.x = (255 - 76);
        ctx->spritePos.y = 120;
        ManagedSprite_SetPriority(
            ctx->currentSprite,
            ov07_0221FAE8(system) + 1);
        ctx->dir = -1;
    } else {
        if (ov07_0223192C(system, ov07_0221C468(system))
            == BTLSCR_ENEMY) {
            ctx->currentSprite = ctx->sprites[1];
            ManagedSprite_SetDrawFlag(ctx->sprites[0], 0);
            ManagedSprite_SetAnim(ctx->currentSprite, 1);
            ctx->spritePos.x = 144;
            ctx->spritePos.y = 64;
        } else {
            ctx->currentSprite = ctx->sprites[0];
            ManagedSprite_SetDrawFlag(ctx->sprites[1], 0);
            ManagedSprite_SetAnim(ctx->currentSprite, 0);
            ctx->spritePos.x = 76;
            ctx->spritePos.y = 120;
            ManagedSprite_SetPriority(
                ctx->currentSprite,
                ov07_0221FAE8(system) + 1);
        }

        ctx->dir = 1;
    }

    ManagedSprite_SetPositionXY(ctx->currentSprite, ctx->spritePos.x, ctx->spritePos.y);
    ManagedSprite_SetAffineOverwriteMode(ctx->currentSprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetOamMode(ctx->currentSprite, GX_OAM_MODE_XLU);

    ov07_02222590(
        &ctx->scale,
        100 * ctx->dir,
        100 * ctx->dir,
        100,
        5,
        100,
        1);
    ov07_0222260C(&ctx->scale);

    f32 scaleX, scaleY;
    ov07_02222644(&ctx->scale, &scaleX, &scaleY);
    ManagedSprite_SetAffineScale(ctx->currentSprite, scaleX, scaleY);

    s16 offsetY = ov07_02222674(
        ctx->spritePos.y,
        16,
        ctx->scale.data[XY_PARAM_CUR_Y]);
    ManagedSprite_SetPositionXY(ctx->currentSprite, ctx->spritePos.x, ctx->spritePos.y + offsetY);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02229480, ctx);
}
