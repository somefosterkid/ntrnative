#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimSystem;

int ov07_0221FAE8(BattleAnimSystem *system);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

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

typedef struct FollowMeSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    ValueLerpContext tilt;
    int state;
} FollowMeSpriteContext;

void ov07_02226880(SysTask *task, void *param);

void ov07_022269D8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022269D8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    FollowMeSpriteContext *ctx = ov07_022324D8(system, sizeof(FollowMeSpriteContext));

    ov07_02231FE4(system, &ctx->common);

    ctx->sprite = sprite;
    ctx->state = 0;

    int defender = ov07_0221C470(system);
    s16 defenderX = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_X);
    s16 defenderY = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_Y);

    ManagedSprite_SetPositionXY(ctx->sprite, defenderX, defenderY);

    if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        ManagedSprite_SetAnim(ctx->sprite, 1);
    } else {
        ManagedSprite_SetAnim(ctx->sprite, 0);
    }

    ManagedSprite_SetPriority(ctx->sprite, ov07_0221FAE8(ctx->common.battleAnimSys));
    ManagedSprite_OffsetPositionXY(ctx->sprite, 0, +12);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226880, ctx);
}

typedef struct OffsetAndAnimateSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
} OffsetAndAnimateSpriteContext;

void ov07_02226A68(SysTask *task, void *param);

void ov07_02226A68(SysTask *task, void *param) {
    OffsetAndAnimateSpriteContext *ctx = param;

    if (!ManagedSprite_IsAnimated(ctx->sprite)) {
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->sprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}
