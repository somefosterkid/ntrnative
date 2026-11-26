#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_74_main_menu_util.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"

struct BattleAnimSystem;

struct BgConfig;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct BgConfig BgConfig;

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

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


typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_022227D8(XYTransformContext *ctx);

typedef struct MagnitudeContext {
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo unused;
    Point2D battlerPositions[4];
    Pokepic *battlerSprites[4];
    int power;
    XYTransformContext shake;
} MagnitudeContext;

enum MagnitudeState {
    MAGNITUDE_STATE_INIT = 0,
    MAGNITUDE_STATE_SHAKE,
};

static void ov07_02229024(SysTask *task, void *param1);

static void ov07_02229024(SysTask *task, void *param1) {
    MagnitudeContext *ctx = param1;

    switch (ctx->common.state) {
    case MAGNITUDE_STATE_INIT:
        ov07_022227A8(
            &ctx->shake,
            2 + ctx->power,
            ctx->power,
            0,
            10);
        ctx->common.state++;
        break;
    case MAGNITUDE_STATE_SHAKE:
        if (ov07_022227D8(&ctx->shake) == 0) {
            ctx->common.state++;
        } else {
            for (int battler = 0; battler < 4; battler++) {
                if (ctx->battlerSprites[battler] == 0L) {
                    continue;
                }

                Pokepic_SetAttr(
                    ctx->battlerSprites[battler],
                    MON_SPRITE_X_CENTER,
                    ctx->battlerPositions[battler].x + ctx->shake.x);
            }

            BgSetPosTextAndCommit(ctx->common.bgConfig, BG_LAYER_MAIN_3, BG_OFFSET_UPDATE_SET_X, ctx->shake.x);
            BgSetPosTextAndCommit(ctx->common.bgConfig, BG_LAYER_MAIN_3, BG_OFFSET_UPDATE_SET_Y, ctx->shake.y);
        }
        break;
    default:
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

enum BattleAnimMoveInfoType {
    BATTLE_ANIM_MOVE_INFO_DAMAGE = 0,
    BATTLE_ANIM_MOVE_INFO_POWER,
    BATTLE_ANIM_MOVE_INFO_FRIENDSHIP,
    BATTLE_ANIM_MOVE_INFO_FIELD_CONDITIONS,
    BATTLE_ANIM_MOVE_INFO_EFFECT_CHANCE,
    BATTLE_ANIM_MOVE_INFO_TERRAIN,

    BATTLE_ANIM_MOVE_INFO_COUNT
};

int ov07_0221C768(BattleAnimSystem *system, enum BattleAnimMoveInfoType type);

void ov07_02231FA0(Pokepic *sprite, Point2D *pos);

void ov07_022290BC(BattleAnimSystem *system);

void ov07_022290BC(BattleAnimSystem *system) {
    MagnitudeContext *ctx = ov07_022324D8(system, sizeof(MagnitudeContext));
    ov07_02231FE4(system, &ctx->common);

    int power = ov07_0221C768(ctx->common.battleAnimSys, BATTLE_ANIM_MOVE_INFO_POWER);
    switch (power) {
    case 150:
        ctx->power = 6;
        break;
    case 110:
        ctx->power = 5;
        break;
    case 90:
        ctx->power = 4;
        break;
    case 70:
        ctx->power = 3;
        break;
    case 50:
        ctx->power = 2;
        break;
    case 30:
        ctx->power = 1;
        break;
    default:
        ctx->power = 0;
        break;
    }

    for (int battler = 0; battler < 4; battler++) {
        ctx->battlerSprites[battler] = ov07_0221FA48(ctx->common.battleAnimSys, battler);
        if (ctx->battlerSprites[battler] == 0L) {
            continue;
        }

        ov07_02231FA0(ctx->battlerSprites[battler], &ctx->battlerPositions[battler]);
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02229024, ctx);
}
