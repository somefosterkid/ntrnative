#include "global.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_07_battle_anim_funcs.h"

struct BattleAnimContext;

struct BattleAnimSystem;

struct BattleBgAnim;

struct BgTrackingTask;

struct SpriteResourcesHeaderList;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

typedef struct BattleAnimContext BattleAnimContext;

typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

BOOL ov07_0221BFC0(BattleAnimSystem *system);

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C470(BattleAnimSystem *system);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

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

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);

typedef struct FollowMeSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    ValueLerpContext tilt;
    int state;
} FollowMeSpriteContext;

typedef struct FissureSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    ValueLerpContext unused;
} FissureSpriteContext;

void ov07_02226A68(SysTask *task, void *param);

void ov07_02226A9C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02222FC4(BattleAnimSystem *arg0);
void ov07_02222FF4(BattleAnimSystem *arg0);
void ov07_02226C90(BattleAnimSystem *arg0);

void ov07_02226A9C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {

    FissureSpriteContext *ctx = ov07_022324D8(system, sizeof(FollowMeSpriteContext));
    ov07_02231FE4(system, &ctx->common);
    ctx->sprite = sprite;

    int anim = 0;
    int defender = ov07_0221C470(system);
    s16 spriteX = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_X);
    s16 spriteY = ov07_02221F80(system, defender, BATTLE_ANIM_POSITION_MON_Y);

    if (ov07_0221BFC0(system) == 1) {
        spriteX = 72;
        spriteY = 32;
        anim = 0;
    } else {
        if (ov07_0223192C(ctx->common.battleAnimSys, defender) == BTLSCR_PLAYER) {
            spriteY = 126;
            anim = 1;
        } else {
            spriteY = 32;
            anim = 0;
        }
    }

    ManagedSprite_SetPositionXY(ctx->sprite, spriteX, spriteY);
    ManagedSprite_SetAnim(ctx->sprite, anim);

    ManagedSprite_SetPriority(ctx->sprite, ov07_0221FB04(ctx->common.battleAnimSys, 2));
    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226A68, ctx);
}

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

BOOL ManagedSprite_IsAnimated(ManagedSprite *managedSprite);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

void ov07_0221C448(BattleAnimSystem *system, SysTask *task);

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);

BOOL ov07_022223CC(XYTransformContext *ctx, XYTransformContext *revs, ManagedSprite *sprite);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void Heap_Free(void *ptr);

typedef struct EscapeItemSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
    u8 spriteAlpha;
    u8 bgAlpha;
    XYTransformContext pos[2];
} EscapeItemSpriteContext;

enum EscapeItemSpriteState {
    ESCAPE_ITEM_SPRITE_STATE_INIT = 0,
    ESCAPE_ITEM_SPRITE_STATE_MOVE,
    ESCAPE_ITEM_SPRITE_STATE_WAIT_FOR_ANIM,
    ESCAPE_ITEM_SPRITE_STATE_FADEOUT,
};

void ov07_02226B2C(SysTask *task, void *param);

void ov07_02226B2C(SysTask *task, void *param) {
    EscapeItemSpriteContext *ctx = param;

    switch (ctx->common.state) {
    case ESCAPE_ITEM_SPRITE_STATE_INIT:
        ov07_02222338(
            &ctx->pos[0],
            &ctx->pos[1],
            (-30),
            112,
            160,
            112,
            21,
            ((fx32)(((64) > 0) ? ((64) * (1 << 12) + 0.5f) : ((64) * (1 << 12) - 0.5f))));
        ctx->common.state++;
        break;
    case ESCAPE_ITEM_SPRITE_STATE_MOVE:
        if (ov07_022223CC(&ctx->pos[0], &ctx->pos[1], ctx->sprite) == 0) {
            ctx->common.state++;
        }
        break;
    case ESCAPE_ITEM_SPRITE_STATE_WAIT_FOR_ANIM: {
        if (!ManagedSprite_IsAnimated(ctx->sprite)) {
            ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
            ctx->spriteAlpha = 16;
            ctx->bgAlpha = 0;
            ov07_02231E08(ctx->common.battleAnimSys, ctx->spriteAlpha, ctx->bgAlpha);
            ctx->common.state++;
        }
        break;
    }
    case ESCAPE_ITEM_SPRITE_STATE_FADEOUT:
        if (ctx->spriteAlpha > 0) {
            ctx->spriteAlpha--;
        }

        if (ctx->bgAlpha < 16) {
            ctx->bgAlpha++;
        }

        G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);

        if (ctx->spriteAlpha == 0) {
            ctx->common.state++;
        }
        break;
    default:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->sprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

void ov07_02226C4C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02226C4C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    EscapeItemSpriteContext *ctx = ov07_022324D8(system, sizeof(EscapeItemSpriteContext));
    ov07_02231FE4(system, &ctx->common);
    ctx->sprite = sprite;

    ManagedSprite_SetPositionXY(ctx->sprite, (-30), 160);
    ManagedSprite_SetPriority(ctx->sprite, ov07_0221FB04(ctx->common.battleAnimSys, 2));

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226B2C, ctx);
}

void ov07_02226C90(BattleAnimSystem *arg0) {
    if (ov07_0221C4A8(arg0, 0) != 0) {
        ov07_02222FC4(arg0);
        return;
    }
    ov07_02222FF4(arg0);
}
