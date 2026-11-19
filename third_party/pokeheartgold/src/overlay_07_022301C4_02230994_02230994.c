#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_megahorn_attacker_context.h"

struct BattleAnimSystem;

ManagedSprite *ov07_0221C4C0(BattleAnimSystem *system, int index);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

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

typedef struct MegahornContext {
    BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    XYTransformContext pos;
    ManagedSprite *hornSprite;
} MegahornContext;

void ov07_02230960(SysTask *task, void *param);

void ov07_02230994(BattleAnimSystem *system);

void ov07_02230994(BattleAnimSystem *system) {
    MegahornContext *ctx = ov07_022324D8(system, sizeof(MegahornContext));

    ctx->battleAnimSys = system;
    ctx->spriteManager = ov07_0221C528(system);

    int attacker = ov07_0221C468(system);

    if (ov07_0223192C(system, attacker) == BTLSCR_PLAYER) {
        ctx->hornSprite = ov07_0221C4C0(system, 0);
        Sprite_DeleteAndFreeResources(ov07_0221C4C0(system, 1));
    } else {
        ctx->hornSprite = ov07_0221C4C0(system, 1);
        Sprite_DeleteAndFreeResources(ov07_0221C4C0(system, 0));
    }

    int dirX = ov07_02222004(system, attacker);
    int dirY = ov07_0222202C(system, attacker);
    int hornX = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_X);
    int hornY = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_Y);

    hornX += 64 * dirX;
    hornY += (-16) * dirY;

    ManagedSprite_SetPositionXY(ctx->hornSprite, hornX, hornY);

    ov07_02222268(
        &ctx->pos,
        hornX,
        hornX + 48 * dirX,
        hornY,
        hornY + (-16) * dirY,
        6);

    ov07_0221C410(ctx->battleAnimSys, ov07_02230960, ctx);
}

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


BOOL ov07_02222314(XYTransformContext *ctx, Pokepic *sprite);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_0222283C(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

enum MegahornAttackerState {
    MEGAHORN_ATTACKER_STATE_INIT = 0,
    MEGAHORN_ATTACKER_STATE_SHAKE,
    MEGAHORN_ATTACKER_STATE_MOVE,
    MEGAHORN_ATTACKER_STATE_WAIT,
    MEGAHORN_ATTACKER_STATE_MOVE_BACK,
    MEGAHORN_ATTACKER_STATE_CLEANUP,
};

void ov07_02230A70(SysTask *task, void *param);

void ov07_02230A70(SysTask *task, void *param) {
    MegahornAttackerContext *ctx = param;

    switch (ctx->state) {
    case MEGAHORN_ATTACKER_STATE_INIT:
        ov07_022227A8(
            &ctx->shake,
            4,
            0,
            1,
            4);
        ctx->state++;
        break;
    case MEGAHORN_ATTACKER_STATE_SHAKE:
        if (ov07_0222283C(&ctx->shake, ctx->x, ctx->y, ctx->sprite) == 0) {
            ctx->state++;
            ov07_02222268(
                &ctx->pos,
                ctx->x,
                ctx->x + (40 * ctx->dirX),
                ctx->y,
                ctx->y + ((-7) * ctx->dirY),
                4);
        }
        break;
    case MEGAHORN_ATTACKER_STATE_MOVE:
        if (ov07_02222314(&ctx->pos, ctx->sprite) == 0) {
            ctx->state++;
            ctx->delay = 8;
        }
        break;
    case MEGAHORN_ATTACKER_STATE_WAIT:
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->state++;
            ov07_02222268(
                &ctx->pos,
                ctx->x + (40 * ctx->dirX),
                ctx->x,
                ctx->y + ((-7) * ctx->dirY),
                ctx->y,
                4);
        }
        break;
    case MEGAHORN_ATTACKER_STATE_MOVE_BACK:
        if (ov07_02222314(&ctx->pos, ctx->sprite) == 0) {
            ctx->state++;
        }
        break;
    case MEGAHORN_ATTACKER_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_X_CENTER, ctx->x);
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_Y_CENTER, ctx->y);
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        break;
    }
}
