#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimSystem;

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

BOOL ov07_02222440(ValueLerpContext *ctx);

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

typedef struct MetronomeSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    ManagedSprite *sprite;
    XYTransformContext scale;
    ValueLerpContext angle;
    int counter;
    int nextEndAngle;
    int scaleDir;
} MetronomeSpriteContext;

enum MetronomeSpriteState {
    METRONOME_SPRITE_STATE_GROW = 0,
    METRONOME_SPRITE_STATE_WIGGLE,
    METRONOME_SPRITE_STATE_SHRINK,
    METRONOME_SPRITE_STATE_CLEANUP,
};

static void ov07_0222D2B0(SysTask *task, void *param);

static void ov07_0222D2B0(SysTask *task, void *param) {
    MetronomeSpriteContext *ctx = param;
    f32 scaleX, scaleY;

    switch (ctx->state) {
    case METRONOME_SPRITE_STATE_GROW: {
        BOOL active = ov07_02222558(&ctx->scale);
        if (active == 1) {
            ov07_02222644(&ctx->scale, &scaleX, &scaleY);
            ManagedSprite_SetAffineScale(ctx->sprite, scaleX * ctx->scaleDir, scaleY);
        } else {
            ctx->state++;
            ctx->counter = 5;

            ov07_02222508(
                &ctx->scale,
                10,
                10,
                1,
                8);
        }
        break;
    }
    case METRONOME_SPRITE_STATE_WIGGLE: {
        BOOL active = ov07_02222440(&ctx->angle);
        ManagedSprite_SetAffineZRotation(ctx->sprite, ctx->angle.value);

        if (active == 0) {
            if (ctx->counter > 0) {
                ctx->counter--;

                int start = ctx->angle.value;
                int end = ctx->nextEndAngle;
                ctx->nextEndAngle = ctx->angle.value;

                ov07_022223F0(&ctx->angle, start, end, 4);
            } else {
                ctx->state++;
            }
        }
        break;
    }
    case METRONOME_SPRITE_STATE_SHRINK: {
        BOOL active = ov07_02222558(&ctx->scale);
        if (active == 1) {
            ov07_02222644(&ctx->scale, &scaleX, &scaleY);
            ManagedSprite_SetAffineScale(ctx->sprite, scaleX * ctx->scaleDir, scaleY);
        } else {
            ctx->state++;
        }
        break;
    }
    case METRONOME_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

BOOL ov07_0221FB78(BattleAnimSystem *system, void *battlerRole);

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

void ov07_0222D3BC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222D3BC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    MetronomeSpriteContext *ctx = ov07_022324D8(system, sizeof(MetronomeSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    int translationDir;
    if (ov07_0221FB78(ctx->battleAnimSys, 0) == 1) {
        ctx->scaleDir = -1;
        translationDir = dir * -1;
    } else {
        ctx->scaleDir = 1;
        translationDir = dir;
    }

    s16 x = ov07_02221F80(system, ov07_0221C468(ctx->battleAnimSys), BATTLE_ANIM_POSITION_MON_X);
    s16 y = ov07_02221F80(system, ov07_0221C468(ctx->battleAnimSys), BATTLE_ANIM_POSITION_MON_Y);

    ctx->sprite = sprite;

    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetAffineScale(ctx->sprite, 1.0f, 1.0f);
    ManagedSprite_SetPositionXY(ctx->sprite, x, y);
    ManagedSprite_OffsetPositionXY(ctx->sprite, 40 * dir, 0);
    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_SetAffineTranslation(
        ctx->sprite,
        (-12) * translationDir,
        12);

    if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_ENEMY) {
        ManagedSprite_SetAnim(ctx->sprite, 1);
    } else {
        ManagedSprite_SetAnim(ctx->sprite, 0);
    }

    ov07_02222508(
        &ctx->scale,
        1,
        10,
        10,
        8);

    if (translationDir >= 0) {
        ov07_022223F0(
            &ctx->angle,
            (((359) * 0xFFFF) / 360),
            (((320) * 0xFFFF) / 360),
            4);
        ctx->nextEndAngle = (((359) * 0xFFFF) / 360);
    } else {
        ov07_022223F0(
            &ctx->angle,
            (((0) * 0xFFFF) / 360),
            (((40) * 0xFFFF) / 360),
            4);
        ctx->nextEndAngle = (((0) * 0xFFFF) / 360);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222D2B0, ctx);
}

typedef struct ConstrictVine {
    ManagedSprite *sprite;
    XYTransformContext scale;
    BOOL flip;
    int scaleCounter;
    int nextStartScale;
    int nextEndScale;
} ConstrictVine;

void ov07_0222D4F4(ConstrictVine *vine, ManagedSprite *sprite, s16 x, s16 y, BOOL flip);

void ov07_0222D4F4(ConstrictVine *vine, ManagedSprite *sprite, s16 x, s16 y, BOOL flip) {
    vine->sprite = sprite;

    ov07_02222508(
        &vine->scale,
        10,
        10,
        8,
        8);

    vine->scaleCounter = 3;
    vine->nextStartScale = 8;
    vine->nextEndScale = 10;
    vine->flip = flip;

    ManagedSprite_SetPositionXY(vine->sprite, x, y);
    ManagedSprite_SetPositionXY(vine->sprite, x, y);
    ManagedSprite_SetAffineOverwriteMode(vine->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetDrawPriority(vine->sprite, 100);
    ManagedSprite_SetPriority(vine->sprite, 1);

    if (flip) {
        ManagedSprite_SetAffineScale(vine->sprite, -1.0f, 1.0f);
    }

    ManagedSprite_SetDrawFlag(vine->sprite, 0);
}
