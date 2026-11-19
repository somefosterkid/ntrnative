#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "pokepic.h"

struct BattleAnimSystem;

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

typedef struct FakeOutSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    ManagedSprite *sprite;
    AlphaFadeContext alpha;
} FakeOutSpriteContext;

enum FakeOutSpriteState {
    FAKE_OUT_SPRITE_STATE_INIT = 0,
    FAKE_OUT_SPRITE_STATE_FADE_IN,
    FAKE_OUT_SPRITE_STATE_WAIT_ANIM,
    FAKE_OUT_SPRITE_STATE_FADE_OUT,
    FAKE_OUT_SPRITE_STATE_CLEANUP,
};

static void ov07_0223061C(SysTask *task, void *param);

static void ov07_0223061C(SysTask *task, void *param) {
    FakeOutSpriteContext *ctx = param;

    switch (ctx->state) {
    case FAKE_OUT_SPRITE_STATE_INIT:
        ov07_02231E08(
            ctx->battleAnimSys,
            0,
            16 - 0);
        ov07_02222AC4(
            &ctx->alpha,
            0,
            16,
            16 - 0,
            16 - 16,
            8);
        ctx->state++;
        break;
    case FAKE_OUT_SPRITE_STATE_FADE_IN:
        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
        }
        break;
    case FAKE_OUT_SPRITE_STATE_WAIT_ANIM:
        if (ManagedSprite_IsAnimated(ctx->sprite) == 0) {
            ctx->state++;
            ov07_02222AC4(
                &ctx->alpha,
                16,
                0,
                16 - 16,
                16 - 0,
                8);
        }
        break;
    case FAKE_OUT_SPRITE_STATE_FADE_OUT:
        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->state++;
        }
        break;
    case FAKE_OUT_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_022306D8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022306D8(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    FakeOutSpriteContext *ctx = ov07_022324D8(system, sizeof(FakeOutSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->sprite = sprite;

    ManagedSprite_SetAnimateFlag(ctx->sprite, TRUE);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);

    ov07_0221C410(ctx->battleAnimSys, ov07_0223061C, ctx);
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

enum BattleAnimAnchorType {
    BATTLE_ANIM_ANCHOR_BOTTOM = 0,
    BATTLE_ANIM_ANCHOR_TOP,
};

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

void ov07_022226C4(Pokepic *sprite, s16 y, s32 height, fx32 scale, enum BattleAnimAnchorType anchor);

BOOL ov07_02222788(XYTransformContext *ctx, Pokepic *sprite);

BOOL ov07_0222283C(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

typedef struct FakeOutContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    s16 defenderY;
    s16 defenderX;
    s16 defenderHeight;
    Pokepic *defenderSprite;
    XYTransformContext scale;
    XYTransformContext shake;
    int sx;
    int ex;
    int sy;
    int ey;
    int refScale;
    int cycles;
    int frames;
} FakeOutContext;

enum FakeOutState {
    FAKE_OUT_STATE_INIT_SCALE = 0,
    FAKE_OUT_STATE_SCALE,
    FAKE_OUT_STATE_INIT_RESTORE,
    FAKE_OUT_STATE_RESTORE,
    FAKE_OUT_STATE_CLEANUP,
};

static void ov07_02230714(SysTask *task, void *param);

static void ov07_02230714(SysTask *task, void *param) {
    FakeOutContext *ctx = param;
    BOOL active;

    switch (ctx->state) {
    case FAKE_OUT_STATE_INIT_SCALE:
        ov07_02222590(
            &ctx->scale,
            ctx->sx,
            ctx->ex,
            ctx->sy,
            ctx->ey,
            ctx->refScale,
            ((ctx->frames) >> 16));
        ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite);
        ov07_02222788(&ctx->scale, ctx->defenderSprite);
        ov07_022226C4(
            ctx->defenderSprite,
            ctx->defenderY,
            ctx->defenderHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);
        ctx->state++;
        break;
    case FAKE_OUT_STATE_SCALE:
        ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite);
        active = ov07_02222788(&ctx->scale, ctx->defenderSprite);
        ov07_022226C4(
            ctx->defenderSprite,
            ctx->defenderY,
            ctx->defenderHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);

        if (active == 0) {
            ctx->state++;
        }
        break;
    case FAKE_OUT_STATE_INIT_RESTORE:
        ov07_02222590(
            &ctx->scale,
            ctx->ex,
            ctx->sx,
            ctx->ey,
            ctx->sy,
            ctx->refScale,
            ((ctx->frames) & 0xFFFF));
        ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite);
        ov07_02222788(&ctx->scale, ctx->defenderSprite);
        ov07_022226C4(
            ctx->defenderSprite,
            ctx->defenderY,
            ctx->defenderHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);
        ctx->state++;
        break;
    case FAKE_OUT_STATE_RESTORE:
        ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite);
        active = ov07_02222788(&ctx->scale, ctx->defenderSprite);
        ov07_022226C4(
            ctx->defenderSprite,
            ctx->defenderY,
            ctx->defenderHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);

        if (active == 0) {
            ctx->cycles--;

            if (ctx->cycles <= 0) {
                ctx->state++;
            } else {
                ctx->state = 0;
            }
        }
        break;
    case FAKE_OUT_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderX);
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER, ctx->defenderY);

        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_SCALE_X, (1 << 8));
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_SCALE_Y, (1 << 8));

        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }
}

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

void ov07_022308E4(BattleAnimSystem *system);

void ov07_022308E4(BattleAnimSystem *system) {
    FakeOutContext *ctx = ov07_022324D8(system, sizeof(FakeOutContext));

    ctx->battleAnimSys = system;
    ctx->defenderHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);
    ctx->defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    ctx->sx = 10;
    ctx->ex = 7;
    ctx->sy = 10;
    ctx->ey = 30;
    ctx->refScale = 10;
    ctx->cycles = 1;
    ctx->frames = (((4 & 0xFFFF) << 16) | (3 & 0xFFFF));

    ov07_022227A8(
        &ctx->shake,
        2,
        0,
        1,
        2);
    ov07_0221C410(ctx->battleAnimSys, ov07_02230714, ctx);
}
