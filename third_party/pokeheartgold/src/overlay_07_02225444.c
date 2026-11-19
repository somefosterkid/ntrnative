#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
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

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

BOOL ov07_02222240(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

typedef struct RevolveBattlerContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    XYTransformContext revs;
} RevolveBattlerContext;

void ov07_02225444(SysTask *task, void *param);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};
s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);
void ov07_02225640(BattleAnimSystem *system);
void ov07_02225768(BattleAnimSystem *system);

void ov07_02225444(SysTask *task, void *param) {
    RevolveBattlerContext *ctx = param;

    BOOL ative = ov07_02222240(&ctx->revs, ctx->spriteInfo.pos.x, ctx->spriteInfo.pos.y, ctx->spriteInfo.monSprite);
    if (ative == 0) {
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER, ctx->spriteInfo.pos.x);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->spriteInfo.pos.y + (-8));
        ov07_0221C448(ctx->common.battleAnimSys, task);
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

void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);

void ov07_0222548C(BattleAnimSystem *system);

void ov07_0222548C(BattleAnimSystem *system) {
    RevolveBattlerContext *ctx = ov07_022324D8(system, sizeof(RevolveBattlerContext));
    ov07_02231FE4(system, &ctx->common);

    int spriteCount;
    ov07_02232020(system, ov07_0221C4A8(system, 0), &ctx->spriteInfo, &spriteCount);

    ctx->spriteInfo.pos.y -= (-8);

    ov07_02222A44(
        &ctx->revs,
        ov07_0221C4A8(system, 1),
        ov07_0221C4A8(system, 2));

    ctx->revs.data[XY_PARAM_REV_RADIUS_X] /= 2;
    ctx->revs.data[XY_PARAM_REV_RADIUS_Y] /= 2;

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02225444, ctx);
}

BOOL ov07_022222B4(XYTransformContext *ctx);

typedef struct MoveBattlerOffScreenContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    XYTransformContext pos;
    Point2D unused;
    Point2D destPos;
} MoveBattlerOffScreenContext;

void ov07_02225504(SysTask *task, void *param);

void ov07_02225504(SysTask *task, void *param) {
    MoveBattlerOffScreenContext *ctx = param;

    switch (ctx->common.state) {
    case 0:
        if (ov07_022222B4(&ctx->pos) == FALSE) {
            ctx->common.state++;
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER, ctx->pos.x);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->pos.y);
        break;
    default:
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

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

int ov07_0223197C(BattleAnimSystem *system, int battler);

void ov07_02231A50(BattleAnimSystem *system, int battler, Point2D *pos);

void ov07_0222554C(BattleAnimSystem *system);

void ov07_0222554C(BattleAnimSystem *system) {

    int target, battler, frames;

    MoveBattlerOffScreenContext *ctx = ov07_022324D8(system, sizeof(MoveBattlerOffScreenContext));
    ov07_02231FE4(system, &ctx->common);

    target = ov07_0221C4A8(system, 0);
    frames = ov07_0221C4A8(system, 1);

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

    ov07_02231A50(system, battler, &ctx->unused);

    int spriteCount;
    ov07_02232020(system, target, &ctx->spriteInfo, &spriteCount);

    ctx->destPos.x = 0;
    ctx->destPos.y = 0;

    if (ov07_0223192C(ctx->common.battleAnimSys, battler) == BTLSCR_PLAYER) {
        ctx->destPos.x = -(10 * 8);
    } else {
        ctx->destPos.x = 256 + (10 * 8);
    }

    ov07_02222268(&ctx->pos, ctx->spriteInfo.pos.x, ctx->destPos.x, ctx->spriteInfo.pos.y, ctx->spriteInfo.pos.y, frames);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_02225504, ctx);
}

void ov07_02225640(BattleAnimSystem *system) {
    int mode, battler, count;
    MoveBattlerOffScreenContext *ctx = ov07_022324D8(system, sizeof(MoveBattlerOffScreenContext));

    ov07_02231FE4(system, &ctx->common);

    mode = ov07_0221C4A8(system, 0);
    int target = ov07_0221C4A8(system, 1);
    int frames = ov07_0221C4A8(system, 2);

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

    ov07_02231A50(system, battler, &ctx->unused);
    ov07_02232020(system, target, &ctx->spriteInfo, &count);

    ctx->destPos.x = 0;
    ctx->destPos.y = 0;

    if (ov07_0223192C(ctx->common.battleAnimSys, battler) == BTLSCR_PLAYER) {
        ctx->destPos.x = -(10 * 8);
    } else {
        ctx->destPos.x = 256 + (10 * 8);
    }

    if (mode == 0) {
        ov07_02222268(&ctx->pos, ctx->spriteInfo.pos.x, ctx->destPos.x, ctx->spriteInfo.pos.y, ctx->spriteInfo.pos.y, frames);
    } else {
        s16 battlerX = ov07_02221F80(system, battler, BATTLE_ANIM_POSITION_MON_X);
        ov07_02222268(&ctx->pos, ctx->destPos.x, battlerX, ctx->spriteInfo.pos.y, ctx->spriteInfo.pos.y, frames);
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02225504, ctx);
}

void ov07_02225768(BattleAnimSystem *system) {

    int battler, count;
    Point2D destPos;
    BattleAnimSpriteInfo spriteInfo;

    int target = ov07_0221C4A8(system, 0);

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

    ov07_02231A50(system, battler, &destPos);
    ov07_02232020(system, target, &spriteInfo, &count);

    s16 height = ov07_0221FAA0(system, battler);
    Pokepic_SetAttr(spriteInfo.monSprite, MON_SPRITE_X_CENTER, destPos.x);
    Pokepic_SetAttr(spriteInfo.monSprite, MON_SPRITE_Y_CENTER, destPos.y + height);
}
