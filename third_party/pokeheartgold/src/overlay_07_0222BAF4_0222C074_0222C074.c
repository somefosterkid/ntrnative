#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_afterimage_context.h"
#include "overlay_07_drill_peck_context.h"
#include "overlay_07_quick_attack_context.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimContext;

struct BattleAnimSystem;

struct BattleBgAnim;

struct BgTrackingTask;

struct ManagedSprite;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteManager;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

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

typedef struct BattleAnimContext BattleAnimContext;

typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

enum ValueLerpParam {
    VALUE_PARAM_STEPS = 0,
    VALUE_PARAM_STEP_SIZE,

    VALUE_PARAM_FX32_STEPS = 0,
    VALUE_PARAM_FX32_STEP_SIZE,
    VALUE_PARAM_FX32_CUR_ANGLE,

    VALUE_PARAM_COS_STEPS = 0,
    VALUE_PARAM_COS_CUR_ANGLE,
    VALUE_PARAM_COS_AMPLITUDE,
    VALUE_PARAM_COS_STEP_SIZE,
};

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

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

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

void ov07_0222BF18(SysTask *task, void *param);

void ov07_0222C074(BattleAnimSystem *system);

void ov07_0222C074(BattleAnimSystem *system) {

    // the DS build allocates sizeof(QuickAttackContext) (0xF0) here, copy-pasted from Quick Attack; the context is 0x4C
    DrillPeckContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(QuickAttackContext));

    ctx->battleAnimSys = system;
    ctx->state = DRILL_PECK_STATE_MOVE_BWD;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);

    ov07_02222268(&ctx->pos, 0, (-32), 0, 0, 3);
    ov07_022223F0(&ctx->angle, 0, (((20) * 0xFFFF) / 360), 8);

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->pos.data[XY_PARAM_STEP_SIZE] *= dir;
    ctx->angle.data[VALUE_PARAM_STEP_SIZE] *= dir;

    int defenderDir = ov07_02222004(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));

    enum Battler attackerSide = ov07_0223192C(system, ov07_0221C468(ctx->battleAnimSys));
    enum Battler defenderSide = ov07_0223192C(system, ov07_0221C470(ctx->battleAnimSys));

    if (attackerSide == BTLSCR_PLAYER && defenderSide == BTLSCR_PLAYER) {
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_PIVOT, ((10 * 8) / 2) * -1);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_PIVOT, (10 * 8) / 2);
    } else if (attackerSide == BTLSCR_ENEMY && defenderSide == BTLSCR_ENEMY) {
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_PIVOT, ((10 * 8) / 2) * +1);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_PIVOT, (10 * 8) / 2);
    } else {
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_PIVOT, ((10 * 8) / 2) * defenderDir);
        Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_PIVOT, (10 * 8) / 2);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222BF18, ctx);
}
