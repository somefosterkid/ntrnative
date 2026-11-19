#include "global.h"

#include "heap.h"
#include "palette.h"
#include "bg_window.h"
#include "overlay_07_vital_throw_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_alpha_fade_context.h"

struct LockOnSpriteContext;

struct ManagedSprite;

struct PaletteData;

struct Pokepic;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteSystem;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ManagedSprite_SetAnim(ManagedSprite *managedSprite, u32 animID);

void ManagedSprite_SetAnimateFlag(ManagedSprite *managedSprite, BOOL animate);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ov07_0221C448(BattleAnimSystem *system, SysTask *task);

int ov07_0221E6C8(BattleAnimSystem *system);

void ov07_0222207C(XYTransformContext *ctx, ManagedSprite *sprite, s16 cx, s16 cy);

BOOL ov07_022222B4(XYTransformContext *ctx);

typedef struct LockOnSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    Pokepic *defenderSprite;
    Pokepic *battlerSprites[4];
    ManagedSprite *sprite;
    XYTransformContext pos;
    s16 baseX;
    s16 baseY;
    int segment;
    int delay;
    int blinkVisible;
} LockOnSpriteContext;

enum LockOnSpriteState {
    LOCK_ON_SPRITE_STATE_DELAY = 0,
    LOCK_ON_SPRITE_STATE_MOVE,
    LOCK_ON_SPRITE_STATE_FLASH,
    LOCK_ON_SPRITE_STATE_FADE_BACK,
    LOCK_ON_SPRITE_STATE_INIT_BLINK,
    LOCK_ON_SPRITE_STATE_BLINK,
    LOCK_ON_SPRITE_STATE_CLEANUP,
};

void ov07_0222E07C(LockOnSpriteContext *ctx);

static void ov07_0222E108(SysTask *task, void *param);

static void ov07_0222E108(SysTask *task, void *param) {
    LockOnSpriteContext *ctx = param;
    int i;

    switch (ctx->state) {
    case LOCK_ON_SPRITE_STATE_DELAY:
        ctx->delay++;

        if (ctx->delay > 1) {
            ov07_0222E07C(ctx);
            ctx->state++;
            ctx->delay = 0;
        }
        break;
    case LOCK_ON_SPRITE_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos)) {
            ov07_0222207C(&ctx->pos, ctx->sprite, ctx->baseX, ctx->baseY);
        } else {
            ctx->segment++;

            if (ctx->segment < 4) {
                ctx->state = LOCK_ON_SPRITE_STATE_DELAY;
                ctx->baseX += ctx->pos.x;
                ctx->baseY += ctx->pos.y;
            } else {
                ctx->state++;
                ctx->delay = 8;

                ManagedSprite_SetAnim(ctx->sprite, 1);
                ManagedSprite_SetAnimateFlag(ctx->sprite, 1);
            }
        }
        break;
    case LOCK_ON_SPRITE_STATE_FLASH:
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->state++;

            PaletteData_BeginPaletteFade(
                ov07_0221FA78(ctx->battleAnimSys),
                (1 << PLTTBUF_MAIN_BG),
                ov07_0221E6C8(ctx->battleAnimSys),
                (-8),
                0,
                16,
                ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));

            for (i = 0; i < 4; i++) {
                if (ctx->battlerSprites[i]) {
                    Pokepic_StartPaletteFade(
                        ctx->battlerSprites[i],
                        14,
                        16,
                        0,
                        ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));
                }
            }
        }
        break;
    case LOCK_ON_SPRITE_STATE_FADE_BACK:
        if (PaletteData_GetSelectedBuffersBitmask(ov07_0221FA78(ctx->battleAnimSys)) == 0) {
            ctx->state++;
            PaletteData_BeginPaletteFade(
                ov07_0221FA78(ctx->battleAnimSys),
                (1 << PLTTBUF_MAIN_BG),
                ov07_0221E6C8(ctx->battleAnimSys),
                (-8),
                16,
                0,
                ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));

            for (i = 0; i < 4; i++) {
                if (ctx->battlerSprites[i]) {
                    Pokepic_StartPaletteFade(
                        ctx->battlerSprites[i],
                        2,
                        0,
                        0,
                        ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));
                }
            }
        }
        break;
    case LOCK_ON_SPRITE_STATE_INIT_BLINK:
        if (PaletteData_GetSelectedBuffersBitmask(ov07_0221FA78(ctx->battleAnimSys)) == 0) {
            ctx->state++;
            ctx->delay = 4;
            ctx->segment = 4;
            ctx->blinkVisible = 0;
        }
        break;
    case LOCK_ON_SPRITE_STATE_BLINK:
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->delay = 4;
            ctx->segment--;

            if (ctx->segment < 0) {
                ctx->state++;
            } else {
                ManagedSprite_SetDrawFlag(ctx->sprite, ctx->blinkVisible);
                ctx->blinkVisible ^= 1;
            }
        }
        break;
    case LOCK_ON_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetDrawPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C470(BattleAnimSystem *system);

enum {
    GX_BLEND_BGALL = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3,
    GX_BLEND_ALL = GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
};

void ov07_0222E2EC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222E2EC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    LockOnSpriteContext *ctx = ov07_022324D8(system, sizeof(LockOnSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));

    for (int i = 0; i < 4; i++) {
        ctx->battlerSprites[i] = ov07_0221FA48(ctx->battleAnimSys, i);
    }

    ctx->sprite = sprite;

    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_GetPositionXY(ctx->sprite, &ctx->baseX, &ctx->baseY);

    ctx->baseX -= 80 / 2;
    ctx->baseY -= 80 / 2;

    ManagedSprite_SetPositionXY(ctx->sprite, ctx->baseX, ctx->baseY);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);

    ctx->delay = (-16);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_ALL, 31, 26);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222E108, ctx);
}

struct PlayfulHopsContext;

struct ValueLerpContext;

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

void Pokepic_SetAttr(Pokepic *monSprite, enum PokemonSpriteAttribute attribute, int value);

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

typedef struct ValueLerpContext {
    s32 value;
    s32 data[4];
} ValueLerpContext;

void ov07_02222418(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

typedef struct PlayfulHopsContext {
    int state;
    BattleAnimSystem *battleAnimSys;
    Pokepic *battlerSprite;
    s16 battlerX;
    s16 battlerY;
    ValueLerpContext angle;
    XYTransformContext pos;
    int hopCount;
    int hopState;
    int hopDelay;
    int flipTilt;
} PlayfulHopsContext;

enum PlayfulHopsHopState {
    PLAYFUL_HOPS_HOP_STATE_TILT = 0,
    PLAYFUL_HOPS_HOP_STATE_MOVE_DOWN,
    PLAYFUL_HOPS_HOP_STATE_MOVE_UP,
    PLAYFUL_HOPS_HOP_STATE_DONE,
};

static void ov07_0222E3A4(PlayfulHopsContext *ctx, BOOL flip);

static void ov07_0222E3A4(PlayfulHopsContext *ctx, BOOL flip) {
    ov07_02222418(
        &ctx->angle,
        0,
        (s16)(((15) * 0xFFFF) / 360),
        3);

    if (flip) {
        ctx->angle.data[VALUE_PARAM_FX32_STEP_SIZE] *= -1;
        Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_X_PIVOT, 16);
    } else {
        Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_X_PIVOT, -16);
    }

    ctx->hopState = PLAYFUL_HOPS_HOP_STATE_TILT;
    ctx->hopDelay = 0;
}

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_02222314(XYTransformContext *ctx, Pokepic *sprite);

BOOL ov07_02222468(ValueLerpContext *ctx);

static BOOL ov07_0222E3E8(PlayfulHopsContext *ctx);

static BOOL ov07_0222E3E8(PlayfulHopsContext *ctx) {
    BOOL done = 0;

    switch (ctx->hopState) {
    case PLAYFUL_HOPS_HOP_STATE_TILT:
        ov07_02222468(&ctx->angle);
        Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_ROTATION_Z, ctx->angle.value);

        ctx->hopDelay--;

        if (ctx->hopDelay < 0) {
            ctx->hopState++;

            ov07_02222268(
                &ctx->pos,
                ctx->battlerX,
                ctx->battlerX,
                ctx->battlerY,
                ctx->battlerY + 2,
                2);
        }
        break;
    case PLAYFUL_HOPS_HOP_STATE_MOVE_DOWN:
        if (ov07_02222468(&ctx->angle)) {
            Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_ROTATION_Z, ctx->angle.value);
        }

        if (ov07_02222314(&ctx->pos, ctx->battlerSprite) == 0) {
            ctx->hopState++;
            ov07_02222418(
                &ctx->angle,
                (s16)ctx->angle.value,
                (s16)(((0) * 0xFFFF) / 360),
                3);

            ov07_02222268(
                &ctx->pos,
                ctx->battlerX,
                ctx->battlerX,
                ctx->battlerY + 2,
                ctx->battlerY,
                2);
        }
        break;
    case PLAYFUL_HOPS_HOP_STATE_MOVE_UP:
        ov07_02222314(&ctx->pos, ctx->battlerSprite);

        if (ov07_02222468(&ctx->angle)) {
            Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_ROTATION_Z, ctx->angle.value);
        } else {
            Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_X_CENTER, ctx->battlerX);
            Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_Y_CENTER, ctx->battlerY);
            Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_ROTATION_Z, 0);
            ctx->hopState++;
        }
        break;
    case PLAYFUL_HOPS_HOP_STATE_DONE:
        done = 1;
        break;
    }

    return done;
}

enum PlayfulHopsState {
    PLAYFUL_HOPS_STATE_DO_HOPS = 0,
    PLAYFUL_HOPS_STATE_CLEANUP,
};

static void ov07_0222E4F8(SysTask *task, void *param);

static void ov07_0222E4F8(SysTask *task, void *param) {
    PlayfulHopsContext *ctx = param;

    switch (ctx->state) {
    case PLAYFUL_HOPS_STATE_DO_HOPS:
        if (ov07_0222E3E8(ctx)) {
            ctx->hopCount++;

            if (ctx->hopCount < 4) {
                ctx->flipTilt ^= 1;
                ov07_0222E3A4(ctx, ctx->flipTilt);
                ov07_0222E3E8(ctx);
            } else {
                ctx->state++;
            }
        }
        break;
    case PLAYFUL_HOPS_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
}

int Pokepic_GetAttr(Pokepic *monSprite, enum PokemonSpriteAttribute attribute);

u16 ov07_0221C468(BattleAnimSystem *system);

s32 ov07_0221C4A8(BattleAnimSystem *system, int index);

void ov07_0222E54C(BattleAnimSystem *system);

void ov07_0222E54C(BattleAnimSystem *system) {
    PlayfulHopsContext *ctx = ov07_022324D8(system, sizeof(PlayfulHopsContext));
    ctx->battleAnimSys = system;

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    if (ov07_0221C4A8(system, 0) == 0) {
        ctx->battlerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
        ctx->battlerX = Pokepic_GetAttr(ctx->battlerSprite, MON_SPRITE_X_CENTER);
        ctx->battlerY = Pokepic_GetAttr(ctx->battlerSprite, MON_SPRITE_Y_CENTER);
    } else {
        ctx->battlerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
        ctx->battlerX = Pokepic_GetAttr(ctx->battlerSprite, MON_SPRITE_X_CENTER);
        ctx->battlerY = Pokepic_GetAttr(ctx->battlerSprite, MON_SPRITE_Y_CENTER);
    }

    Pokepic_SetAttr(ctx->battlerSprite, MON_SPRITE_Y_PIVOT, 50);

    if (dir == 1) {
        ctx->flipTilt = 0;
    } else {
        ctx->flipTilt = 1;
    }

    ov07_0222E3A4(ctx, ctx->flipTilt);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222E4F8, ctx);
}

struct SwaggerContext;

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

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

void ov07_022226C4(Pokepic *sprite, s16 y, s32 height, fx32 scale, enum BattleAnimAnchorType anchor);

BOOL ov07_02222768(XYTransformContext *ctx, Pokepic *sprite);

typedef struct SwaggerContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    Pokepic *sprite;
    XYTransformContext scale;
    int scaleDelay;
    int subState;
    int baseY;
    int spriteHeight;
} SwaggerContext;

enum SwaggerState {
    SWAGGER_STATE_SCALE = 0,
    SWAGGER_STATE_WAIT,
    SWAGGER_STATE_CLEANUP,

    SWAGGER_SUB_STATE_GROW = 0,
    SWAGGER_SUB_STATE_WAIT,
    SWAGGER_SUB_STATE_SHRINK,
    SWAGGER_SUB_STATE_FINISH,
};

static BOOL ov07_0222E5E8(SwaggerContext *ctx);

static BOOL ov07_0222E5E8(SwaggerContext *ctx) {
    BOOL done = 0;

    switch (ctx->subState) {
    case SWAGGER_SUB_STATE_GROW:
        if (ov07_02222768(&ctx->scale, ctx->sprite)) {
            ov07_022226C4(
                ctx->sprite,
                ctx->baseY,
                (s16)ctx->spriteHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->subState++;
        }
        break;
    case SWAGGER_SUB_STATE_WAIT:
        ctx->scaleDelay--;
        if (ctx->scaleDelay < 0) {
            ctx->subState++;
            ov07_02222508(
                &ctx->scale,
                15,
                10,
                10,
                8);
        }
        break;
    case SWAGGER_SUB_STATE_SHRINK:
        if (ov07_02222768(&ctx->scale, ctx->sprite)) {
            ov07_022226C4(
                ctx->sprite,
                ctx->baseY,
                (s16)ctx->spriteHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->subState++;
        }
        break;
    case SWAGGER_SUB_STATE_FINISH:
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_X, (1 << 8));
        Pokepic_SetAttr(ctx->sprite, MON_SPRITE_SCALE_Y, (1 << 8));
        done = 1;
        break;
    }

    return done;
}

static void ov07_0222E6A8(SysTask *task, void *param);

static void ov07_0222E6A8(SysTask *task, void *param) {
    SwaggerContext *ctx = param;

    switch (ctx->state) {
    case SWAGGER_STATE_SCALE:
        if (ov07_0222E5E8(ctx)) {
            ctx->state++;
            ctx->delay = 16;
        }
        break;
    case SWAGGER_STATE_WAIT:
        ctx->delay--;
        if (ctx->delay < 0) {
            ctx->state++;
        }
        break;
    case SWAGGER_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
}

void ov07_0222E6F4(BattleAnimSystem *system);

void ov07_0222E6F4(BattleAnimSystem *system) {
    SwaggerContext *ctx = ov07_022324D8(system, sizeof(SwaggerContext));

    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));

    ov07_02222508(
        &ctx->scale,
        10,
        10,
        15,
        8);

    ctx->scaleDelay = 4;
    ctx->spriteHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->baseY = Pokepic_GetAttr(ctx->sprite, MON_SPRITE_Y_CENTER);

    ov07_0221C410(ctx->battleAnimSys, ov07_0222E6A8, ctx);
}

struct MeanLookSpriteContext;

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

typedef struct MeanLookSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    int timer;
    ManagedSprite *sprite;
    XYTransformContext scale;
    AlphaFadeContext alpha;
    int pulseState;
    int pulseCount;
    int repeatCount;
    int scaleFrames;
    int startScale;
    int endScale;
    int seqState;
} MeanLookSpriteContext;

enum MeanLookSpriteState {
    MEAN_LOOK_SPRITE_STATE_PULSE = 0,
    MEAN_LOOK_SPRITE_STATE_FLASH,
    MEAN_LOOK_SPRITE_STATE_FADE,
    MEAN_LOOK_SPRITE_STATE_WAIT_FADE,
    MEAN_LOOK_SPRITE_STATE_CLEANUP,

    MEAN_LOOK_SPRITE_PULSE_STATE_SHRINK = 0,
    MEAN_LOOK_SPRITE_PULSE_STATE_GROW,
    MEAN_LOOK_SPRITE_PULSE_STATE_WAIT_FADE,
    MEAN_LOOK_SPRITE_PULSE_STATE_DONE,

    MEAN_LOOK_SPRITE_PULSESEQ_STATE_INIT = 0,
    MEAN_LOOK_SPRITE_PULSESEQ_STATE_RUN,
    MEAN_LOOK_SPRITE_PULSESEQ_STATE_DONE,
};

static void ov07_0222E754(MeanLookSpriteContext *ctx);

static void ov07_0222E754(MeanLookSpriteContext *ctx) {
    ov07_02222508(
        &ctx->scale,
        (s16)(ctx->startScale / 100),
        10,
        (s16)(ctx->endScale / 100),
        ctx->scaleFrames / 100);

    ov07_02231E08(
        ctx->battleAnimSys,
        16,
        0);

    ov07_02222AC4(
        &ctx->alpha,
        16,
        10,
        16 - 16,
        16 - 10,
        (ctx->scaleFrames / 100) * 2);

    ctx->pulseState = MEAN_LOOK_SPRITE_PULSE_STATE_SHRINK;
    ctx->pulseCount = 0;
}

BOOL ov07_02222748(XYTransformContext *ctx, ManagedSprite *sprite);

static BOOL ov07_0222E7BC(MeanLookSpriteContext *ctx);

static BOOL ov07_0222E7BC(MeanLookSpriteContext *ctx) {
    BOOL done = 0;

    switch (ctx->pulseState) {
    case MEAN_LOOK_SPRITE_PULSE_STATE_SHRINK:
        if (ov07_02222748(&ctx->scale, ctx->sprite) == 0) {
            ov07_02222508(
                &ctx->scale,
                (s16)(ctx->endScale / 100),
                10,
                (s16)(ctx->startScale / 100),
                ctx->scaleFrames / 100);
            ctx->pulseState++;
        }
        break;
    case MEAN_LOOK_SPRITE_PULSE_STATE_GROW:
        if (ov07_02222748(&ctx->scale, ctx->sprite) == 0) {
            ctx->pulseCount++;

            if (ctx->pulseCount < 2) {
                ctx->pulseState = MEAN_LOOK_SPRITE_PULSE_STATE_SHRINK;
                ov07_02222508(
                    &ctx->scale,
                    (s16)(ctx->startScale / 100),
                    10,
                    (s16)(ctx->endScale / 100),
                    ctx->scaleFrames / 100);
            } else {
                ctx->pulseState++;
            }
        }
        break;
    case MEAN_LOOK_SPRITE_PULSE_STATE_WAIT_FADE:
        if (ov07_02222AF4(&ctx->alpha)) {
            ctx->pulseState++;
            done = 1;
        }
        break;
    case MEAN_LOOK_SPRITE_PULSE_STATE_DONE:
        done = 1;
        break;
    }

    return done;
}

static BOOL ov07_0222E894(MeanLookSpriteContext *ctx);

static BOOL ov07_0222E894(MeanLookSpriteContext *ctx) {
    BOOL done = 0;

    switch (ctx->seqState) {
    case MEAN_LOOK_SPRITE_PULSESEQ_STATE_INIT:
        ov07_0222E754(ctx);
        ov07_0222E7BC(ctx);
        ctx->seqState++;
        break;
    case MEAN_LOOK_SPRITE_PULSESEQ_STATE_RUN:
        if (ov07_0222E7BC(ctx)) {
            ctx->repeatCount--;

            if (ctx->repeatCount >= 0) {
                ctx->scaleFrames -= ctx->scaleFrames / 3;
                ctx->startScale -= ctx->startScale / 3;
                ctx->endScale -= ctx->endScale / 3;
                ctx->seqState = MEAN_LOOK_SPRITE_PULSESEQ_STATE_INIT;
            } else {
                ctx->seqState++;
                done = 1;
            }
        }
        break;
    case MEAN_LOOK_SPRITE_PULSESEQ_STATE_DONE:
        done = 1;
        break;
    }

    return done;
}

BOOL ManagedSprite_IsAnimated(ManagedSprite *managedSprite);

void ManagedSprite_SetAffineScale(ManagedSprite *managedSprite, f32 xScale, f32 yScale);

void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, int planeMask, u32 screenSelect);

BOOL IsBrightnessTransitionActive(u32 screenSelect);

static void ov07_0222E90C(SysTask *task, void *param);

static void ov07_0222E90C(SysTask *task, void *param) {
    MeanLookSpriteContext *ctx = param;

    switch (ctx->state) {
    case MEAN_LOOK_SPRITE_STATE_PULSE:
        if (ov07_0222E894(ctx)) {
            ctx->state++;
            ctx->timer = 30;

            ManagedSprite_SetAnim(ctx->sprite, 0);
            ManagedSprite_SetAnimateFlag(ctx->sprite, 1);
            ManagedSprite_SetAffineScale(ctx->sprite, 1.0f, 1.0f);

            ov07_02222AC4(
                &ctx->alpha,
                0,
                16,
                16,
                0,
                4);
        }
        break;
    case MEAN_LOOK_SPRITE_STATE_FLASH:

        if (ctx->timer == 0) {
            StartBrightnessTransition(
                8,
                16,
                0,
                (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3),
                (1 << SCREEN_MAIN));
            ctx->timer = -1;
        } else {
            if (ctx->timer > 0) {
                ctx->timer--;
            }
        }

        if (ctx->timer == -1) {
            if (IsBrightnessTransitionActive(1 << SCREEN_MAIN)) {
                StartBrightnessTransition(
                    8,
                    0,
                    16,
                    (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3),
                    (1 << SCREEN_MAIN));
                ctx->timer = -2;
            }
        }

        if (ManagedSprite_IsAnimated(ctx->sprite) == 0) {
            ctx->state++;
        }
        break;
    case MEAN_LOOK_SPRITE_STATE_FADE:
        if (IsBrightnessTransitionActive(1 << SCREEN_MAIN)) {
            ctx->state++;
            ov07_02231E08(ctx->battleAnimSys, 16, 0);
            ov07_02222AC4(
                &ctx->alpha,
                16,
                0,
                0,
                16,
                8);
        }
        break;
    case MEAN_LOOK_SPRITE_STATE_WAIT_FADE:
        if (ov07_02222AF4(&ctx->alpha)) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
            ctx->state++;
        }
        break;
    case MEAN_LOOK_SPRITE_STATE_CLEANUP:
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

void ManagedSprite_SetAnimationFrame(ManagedSprite *managedSprite, u16 frame);

void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *managedSprite, u8 mode);

void ov07_0222EA48(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222EA48(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    MeanLookSpriteContext *ctx = ov07_022324D8(system, sizeof(MeanLookSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->sprite = sprite;

    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetAnimationFrame(ctx->sprite, 4);

    ctx->repeatCount = 3;
    ctx->scaleFrames = 4 * 100;
    ctx->startScale = 15 * 100;
    ctx->endScale = 10 * 100;

    ov07_0221C410(ctx->battleAnimSys, ov07_0222E90C, ctx);
}

struct ReturnContext;

typedef struct ReturnContext {
    int state;
    BattleAnimSystem *battleAnimSys;
    Pokepic *attackerSprite;
    s16 attackerX;
    s16 attackerY;
    XYTransformContext pos;
    int moveCount;
} ReturnContext;

static void ov07_0222EAB4(ReturnContext *ctx);

static void ov07_0222EAB4(ReturnContext *ctx) {
    switch (ctx->moveCount) {
    case 0:
    case 2:
        ov07_02222268(
            &ctx->pos,
            0,
            0,
            0,
            32,
            6);
        break;
    case 1:
    case 3:
        ov07_02222268(
            &ctx->pos,
            0,
            0,
            32,
            0,
            6);
        break;
    }
}

void ov07_022220B8(XYTransformContext *ctx, Pokepic *sprite, s16 cx, s16 cy);

enum ReturnState {
    RETURN_STATE_INIT = 0,
    RETURN_STATE_MOVE,
    RETURN_STATE_CLEANUP,
};

static void ov07_0222EB00(SysTask *task, void *param);

static void ov07_0222EB00(SysTask *task, void *param) {
    ReturnContext *ctx = param;

    switch (ctx->state) {
    case RETURN_STATE_INIT:
        ov07_0222EAB4(ctx);
        ctx->state++;
        break;
    case RETURN_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos)) {
            ov07_022220B8(&ctx->pos, ctx->attackerSprite, ctx->attackerX, ctx->attackerY);
        } else {
            ctx->moveCount++;
            if (ctx->moveCount < 4) {
                ctx->state = RETURN_STATE_INIT;
            } else {
                ctx->state++;
            }
        }
        break;
    case RETURN_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_0222EB6C(BattleAnimSystem *system);

void ov07_0222EB6C(BattleAnimSystem *system) {
    ReturnContext *ctx = ov07_022324D8(system, sizeof(ReturnContext));

    ctx->battleAnimSys = system;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);
    ctx->moveCount = 0;

    ov07_0221C410(ctx->battleAnimSys, ov07_0222EB00, ctx);
}

struct VitalThrowContext;

BOOL ov07_02222240(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

void ov07_02222A44(XYTransformContext *ctx, int revs, int stepsPerRev);

enum VitalThrowState {
    VITAL_THROW_STATE_INIT = 0,
    VITAL_THROW_STATE_REVOLVE,
    VITAL_THROW_STATE_MOVE_ATTACKER,
    VITAL_THROW_STATE_MOVE_DEFENDER,
    VITAL_THROW_STATE_MOVE_BACK_ATTACKER,
    VITAL_THROW_STATE_MOVE_BACK_DEFENDER,
    VITAL_THROW_STATE_CLEANUP,
};

void ov07_0222EBB0(SysTask *task, void *param);

void ov07_0222EBB0(SysTask *task, void *param) {
    VitalThrowContext *ctx = param;

    switch (ctx->state) {
    case VITAL_THROW_STATE_INIT:
        ov07_02222A44(
            &ctx->attackerPos,
            1,
            64);
        ctx->attackerPos.data[XY_PARAM_REV_RADIUS_X] *= ctx->dir;
        ctx->state++;
        break;
    case VITAL_THROW_STATE_REVOLVE:
        if (ov07_02222240(
                &ctx->attackerPos,
                ctx->attackerX,
                (s16)(ctx->attackerY - (-8)),
                ctx->attackerSprite)
            == 0) {

            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY);

            ov07_02222268(
                &ctx->attackerPos,
                ctx->attackerX,
                ctx->attackerX + 32,
                0,
                0,
                2);

            ctx->attackerPos.data[XY_PARAM_REV_CUR_X] *= ctx->dir;
            ctx->state++;

            ov07_022222B4(&ctx->attackerPos);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerPos.x);
        }
        break;
    case VITAL_THROW_STATE_MOVE_ATTACKER:
        if (ov07_022222B4(&ctx->attackerPos)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerPos.x);
        } else {
            ov07_02222268(
                &ctx->defenderPos,
                ctx->defenderX,
                ctx->defenderX + (32 * ctx->dir),
                0,
                0,
                2);
            ctx->state++;
        }
        break;
    case VITAL_THROW_STATE_MOVE_DEFENDER:
        if (ov07_022222B4(&ctx->defenderPos)) {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderPos.x);
        } else {
            ov07_02222268(&ctx->attackerPos, ctx->attackerPos.x, ctx->attackerX, 0, 0, 8);
            ctx->state++;
        }
        break;
    case VITAL_THROW_STATE_MOVE_BACK_ATTACKER:
        if (ov07_022222B4(&ctx->attackerPos)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerPos.x);
        } else {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
            ov07_02222268(&ctx->defenderPos, ctx->defenderPos.x, ctx->defenderX, 0, 0, 8);
            ctx->state++;
        }
        break;
    case VITAL_THROW_STATE_MOVE_BACK_DEFENDER:
        if (ov07_022222B4(&ctx->defenderPos)) {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderPos.x);
        } else {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderX);
            ctx->state++;
        }
        break;
    case VITAL_THROW_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
