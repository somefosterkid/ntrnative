#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_spite_context.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"
#include "pokepic.h"

struct BattleAnimSystem;

BOOL ov07_02222468(ValueLerpContext *ctx);

BOOL ov07_02222558(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

typedef struct NightShaderAttackerContext {
    BattleAnimSystem *battleAnimSys;
    SpriteManager *pokemonSpriteManager;
    int state;
    int delay;
    ManagedSprite *xluSprite;
    XYTransformContext scale;
    ValueLerpContext alpha;
    BOOL isContest;
} NightShaderAttackerContext;

enum NightShadeAttackerState {
    NIGHT_SHADE_ATTACKER_STATE_FADE_IN = 0,
    NIGHT_SHADE_ATTACKER_STATE_DELAY,
    NIGHT_SHADE_ATTACKER_STATE_GROW,
    NIGHT_SHADE_ATTACKER_STATE_CLEANUP,
};

void ov07_0222CBFC(SysTask *task, void *param);

s32 ov07_0222CFCC(SpiteContext *ctx);

void ov07_0222CBFC(SysTask *task, void *param) {
    NightShaderAttackerContext *ctx = param;
    BOOL active;
    f32 scaleX, scaleY;

    switch (ctx->state) {
    case NIGHT_SHADE_ATTACKER_STATE_FADE_IN:
        active = ov07_02222468(&ctx->alpha);
        G2_ChangeBlendAlpha(ctx->alpha.value, 31 - ctx->alpha.value);

        if (active == 0) {
            ctx->state++;
            ctx->delay = 0;
        }
        break;
    case NIGHT_SHADE_ATTACKER_STATE_DELAY:
        ctx->delay++;
        if (ctx->delay > 32) {
            ctx->state++;
        }
        break;
    case NIGHT_SHADE_ATTACKER_STATE_GROW:
        active = ov07_02222558(&ctx->scale);
        ov07_02222644(&ctx->scale, &scaleX, &scaleY);

        if (ctx->isContest == 1) {
            scaleX = -scaleX;
        }

        ManagedSprite_SetAffineScale(ctx->xluSprite, scaleX, scaleY);

        if (active == 0) {
            ManagedSprite_SetDrawFlag(ctx->xluSprite, 0);
            ctx->state++;
        }
        break;
    case NIGHT_SHADE_ATTACKER_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
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


void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

BOOL Pokepic_ResumePaletteFade(Pokepic *monSprite);

BOOL ov07_0221FAC8(BattleAnimSystem *system, int battler);

u8 ov07_02221FF0(ManagedSprite *sprite);

void ov07_0222207C(XYTransformContext *ctx, ManagedSprite *sprite, s16 cx, s16 cy);

void ov07_022220B8(XYTransformContext *ctx, Pokepic *sprite, s16 cx, s16 cy);

BOOL ov07_022227D8(XYTransformContext *ctx);

typedef struct NightShadeDefenderContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    Pokepic *defenderSprite;
    ManagedSprite *managedSprite;
    BOOL animateManagedSprite;
    XYTransformContext shake;
    s16 defenderX;
    s16 defenderY;
    int defenderShadowHeight;
} NightShadeDefenderContext;

enum NightShadeDefenderState {
    NIGHT_SHADE_DEFENDER_STATE_FADE_OUT = 0,
    NIGHT_SHADE_DEFENDER_STATE_FADE_IN,
    NIGHT_SHADE_DEFENDER_STATE_CLEANUP,
};

void ov07_0222CCBC(SysTask *task, void *param);

void ov07_0222CCBC(SysTask *task, void *param) {
    NightShadeDefenderContext *ctx = param;
    BOOL active;

    switch (ctx->state) {
    case NIGHT_SHADE_DEFENDER_STATE_FADE_OUT:
        ov07_022227D8(&ctx->shake);
        ov07_022220B8(&ctx->shake, ctx->defenderSprite, ctx->defenderX, ctx->defenderY);

        if (ctx->animateManagedSprite == 1) {
            ov07_0222207C(
                &ctx->shake,
                ctx->managedSprite,
                ctx->defenderX,
                ctx->defenderY - ctx->defenderShadowHeight);
        }

        if (Pokepic_ResumePaletteFade(ctx->defenderSprite) == 0) {
            Pokepic_StartPaletteFade(
                ctx->defenderSprite,
                16,
                0,
                0,
                (((0) << 10) | ((0) << 5) | (0)));

            if (ctx->animateManagedSprite == 1) {
                int palette = 1 << ov07_02221FF0(ctx->managedSprite);
                PaletteData_BeginPaletteFade(
                    ov07_0221FA78(ctx->battleAnimSys),
                    (1 << PLTTBUF_MAIN_OBJ),
                    palette,
                    0,
                    16,
                    0,
                    (((0) << 10) | ((0) << 5) | (0)));
            }

            ctx->state++;
        }
        break;
    case NIGHT_SHADE_DEFENDER_STATE_FADE_IN:
        active = ov07_022227D8(&ctx->shake);
        ov07_022220B8(&ctx->shake, ctx->defenderSprite, ctx->defenderX, ctx->defenderY);

        if (ctx->animateManagedSprite == 1) {
            ov07_0222207C(
                &ctx->shake,
                ctx->managedSprite,
                ctx->defenderX,
                ctx->defenderY - ctx->defenderShadowHeight);
        }

        if (Pokepic_ResumePaletteFade(ctx->defenderSprite) == 0) {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderX);
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER, ctx->defenderY);

            if (ctx->animateManagedSprite == 1) {
                if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
                    Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_HIDE, 0);
                }

                ManagedSprite_SetPositionXY(ctx->managedSprite, ctx->defenderX, ctx->defenderY - ctx->defenderShadowHeight);
            }

            ctx->state++;
        }
        break;
    case NIGHT_SHADE_DEFENDER_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }
}

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

BOOL ov07_0221BFC0(BattleAnimSystem *system);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

void ov07_02222418(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_0222CE10(BattleAnimSystem *system);

void ov07_0222CE10(BattleAnimSystem *system) {
    NightShaderAttackerContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(NightShaderAttackerContext));
    memset(ctx, 0, sizeof(NightShaderAttackerContext));

    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);
    ctx->isContest = ov07_0221BFC0(system);
    ctx->xluSprite = ov07_0221C4E8(ctx->battleAnimSys, 0);

    ov07_02222508(
        &ctx->scale,
        14,
        10,
        10,
        8);
    ov07_02222418(
        &ctx->alpha,
        0,
        16,
        8);

    ManagedSprite_SetOamMode(ctx->xluSprite, GX_OAM_MODE_XLU);
    ov07_02231E08(ctx->battleAnimSys, 0, 31);
    ManagedSprite_SetAffineOverwriteMode(ctx->xluSprite, AFFINE_OVERWRITE_MODE_DOUBLE);

    if (ctx->isContest == 1) {
        ManagedSprite_SetAffineScale(ctx->xluSprite, -((f32)14 / 10), ((f32)14 / 10));
    } else {
        ManagedSprite_SetAffineScale(ctx->xluSprite, ((f32)14 / 10), ((f32)14 / 10));
    }

    ManagedSprite_SetDrawPriority(ctx->xluSprite, 100);
    ManagedSprite_SetPriority(ctx->xluSprite, 1);

    ov07_0221C410(ctx->battleAnimSys, ov07_0222CBFC, ctx);
}


void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_0222CEC0(BattleAnimSystem *system);

void ov07_0222CEC0(BattleAnimSystem *system) {
    NightShadeDefenderContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(NightShadeDefenderContext));
    memset(ctx, 0, sizeof(NightShadeDefenderContext));

    ctx->battleAnimSys = system;
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ctx->defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);
    ctx->defenderShadowHeight = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_SHADOW_HEIGHT);

    ov07_022227A8(
        &ctx->shake,
        4,
        0,
        1,
        4);

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));

    ctx->shake.x += dir;

    Pokepic_StartPaletteFade(
        ctx->defenderSprite,
        0,
        16,
        0,
        (((0) << 10) | ((0) << 5) | (0)));

    ctx->managedSprite = ov07_0221C4E8(ctx->battleAnimSys, 1);
    int attackerType = ov07_02231924(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    int defenderType = ov07_02231924(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));

    if ((attackerType == 2 && defenderType == 4) || (attackerType == 5 && defenderType == 3)) {
        int palette = 1 << ov07_02221FF0(ctx->managedSprite);
        PaletteData_BeginPaletteFade(
            ov07_0221FA78(ctx->battleAnimSys),
            (1 << PLTTBUF_MAIN_OBJ),
            palette,
            0,
            0,
            16,
            (((0) << 10) | ((0) << 5) | (0)));
        ctx->animateManagedSprite = 1;

        if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_HIDE, 1);
        }
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222CCBC, ctx);
}

s32 ov07_0222CFCC(SpiteContext *ctx) {
    s32 v1;
    s32 v2;
    u32 v0;

    v0 = *((u32 *)&ctx->subState);
    v2 = 0;
    switch (v0) {
    case 0:
        if (ov07_02222AF4(&ctx->alpha) == 1) {
            *((u32 *)&ctx->subState) = (u32)(*((u32 *)&ctx->subState) + 1);
            *((s32 *)&ctx->delay) = 0x20;
        }
        break;
    case 1:
        v1 = *((s32 *)&ctx->delay) - 1;
        *((s32 *)&ctx->delay) = v1;
        if (v1 < 0) {
            *((u32 *)&ctx->subState) = (u32)(*((u32 *)&ctx->subState) + 1);
            ov07_02222AC4(&ctx->alpha, 8, 0, 8, 0x10, 0x18);
        }
        break;
    case 2:
        if (ov07_02222AF4(&ctx->alpha) == 1) {
            *((u32 *)&ctx->subState) = (u32)(*((u32 *)&ctx->subState) + 1);
            ov07_02222D3C(ctx->bgScroll);
        }
        break;
    case 3:
        v2 = 1;
        break;
    }
    return v2;
}
