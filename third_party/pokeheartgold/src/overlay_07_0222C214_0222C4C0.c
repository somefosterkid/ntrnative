#include "global.h"
#include "constants/heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimSystem;

struct KinesisSpoon;

struct KinesisSpriteContext;

struct ManagedSprite;

struct SpriteManager;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct SysTask;

struct SysTaskQueue;

struct ValueLerpContext;

struct XYTransformContext;

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
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

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

void ov07_0222212C(XYTransformContext *ctx, u16 sx, u16 ex, u16 sy, u16 ey, fx32 rx, fx32 ry, u16 stepSizeX);

void ov07_02222494(ValueLerpContext *ctx, u16 start, u16 end, fx32 amplitude, u32 steps);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

typedef struct KinesisSpoon {
    ManagedSprite *sprite;
    XYTransformContext revs;
    ValueLerpContext cosWave;
    s16 revStepSizeX;
    s16 posX;
    s16 posY;
    int state;
    BOOL active;
} KinesisSpoon;

typedef struct KinesisSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    KinesisSpoon spoon;
    KinesisSpoon extraSpoons[2];
    int state;
    int delay;
    int activeSpoon;
    int spoonAlpha;
} KinesisSpriteContext;

void ov07_0222C18C(KinesisSpoon *spoon, SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template, const KinesisSpoon *source);

void ov07_0222C2D4(SysTask *task, void *param);

void ov07_0222C4C0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222C4C0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;
    int i;

    KinesisSpriteContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(KinesisSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->delay = 8;
    ctx->activeSpoon = 0;
    ctx->state = 0;

    int attacker = ov07_0221C468(system);
    int revRadiusX = ov07_02222004(system, attacker);
    revRadiusX *= ((fx32)(((-32) > 0) ? ((-32) * (1 << 12) + 0.5f) : ((-32) * (1 << 12) - 0.5f)));

    ctx->spoon.sprite = sprite;
    ctx->spoon.state = 0;

    ov07_0222212C(
        &ctx->spoon.revs,
        (((90) * 0xFFFF) / 360),
        (((270) * 0xFFFF) / 360),
        (((90) * 0xFFFF) / 360),
        (((270) * 0xFFFF) / 360),
        revRadiusX,
        ((fx32)(((-8) > 0) ? ((-8) * (1 << 12) + 0.5f) : ((-8) * (1 << 12) - 0.5f))),
        (((10) * 0xFFFF) / 360));
    ov07_02222494(
        &ctx->spoon.cosWave,
        (((0) * 0xFFFF) / 360),
        (((360) * 0xFFFF) / 360),
        -1 * ((fx32)0x0000000000001000L),
        ctx->spoon.revs.data[0]);

    ctx->spoon.revStepSizeX = ctx->spoon.revs.data[XY_PARAM_REV_STEP_SIZE_X];
    ctx->spoon.active = 1;
    ctx->spoon.posX = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_X);
    ctx->spoon.posY = ov07_02221F80(system, attacker, BATTLE_ANIM_POSITION_MON_Y);

    ManagedSprite_SetPositionXY(ctx->spoon.sprite, ctx->spoon.posX, ctx->spoon.posY);
    ManagedSprite_SetDrawPriority(ctx->spoon.sprite, 100);
    ManagedSprite_SetPriority(ctx->spoon.sprite, 1);

    template = ov07_0221F9E8(ctx->battleAnimSys);
    template.x = ctx->spoon.posX;
    template.y = ctx->spoon.posY;

    for (i = 0; i < 2; i++) {
        ov07_0222C18C(&ctx->extraSpoons[i], ctx->spriteSys, ctx->spriteMan, &template, &ctx->spoon);
    }

    ctx->spoonAlpha = 0;

    ov07_02231E08(ctx->battleAnimSys, 0, 31);
    ManagedSprite_SetOamMode(ctx->spoon.sprite, GX_OAM_MODE_XLU);

    for (i = 0; i < 2; i++) {
        ManagedSprite_SetOamMode(ctx->extraSpoons[i].sprite, GX_OAM_MODE_XLU);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222C2D4, ctx);
}
