#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_fade_bg_context.h"
#include "overlay_07_scale_pokemon_sprite_context.h"

#include "error_handling.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "pokepic.h"

struct BattleAnimSystem;

int ov07_0221E6C8(BattleAnimSystem *system);

void ov07_02223E94(SysTask *task, void *param);

void ov07_02223EB4(BattleAnimSystem *system);

void ov07_02223EB4(BattleAnimSystem *system) {
    FadeBgContext *ctx = ov07_022324D8(system, sizeof(FadeBgContext));

    ctx->battleAnimSys = system;
    ctx->paletteData = ov07_0221FA78(ctx->battleAnimSys);

    u16 paletteToFade;
    switch (ov07_0221C4A8(system, 0)) {
    case 0:
        paletteToFade = ov07_0221E6C8(system);
        break;
    case 1:
        paletteToFade = (1 << 8);
        break;
    case 2:
        paletteToFade = (1 << 9);
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    PaletteData_BeginPaletteFade(
        ctx->paletteData,
        (1 << PLTTBUF_MAIN_BG),
        paletteToFade,
        ov07_0221C4A8(system, 1),
        ov07_0221C4A8(system, 2),
        ov07_0221C4A8(system, 3),
        ov07_0221C4A8(system, 4));

    ov07_0221C410(ctx->battleAnimSys, ov07_02223E94, ctx);
}

struct Pokepic;

struct PokepicDrawParam;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

BOOL Pokepic_ResumePaletteFade(Pokepic *monSprite);

typedef struct FadeBattlerSpriteContext {
    BattleAnimSystem *battleAnimSys;
    Pokepic *sprite;
    int state;
    int endDelay;
    int fadeDelay;
    int color;
    int alpha;
    int holdDelay;
} FadeBattlerSpriteContext;

enum FadeBattlerSpriteState {
    FADE_BATTLER_SPRITE_STATE_INIT = 0,
    FADE_BATTLER_SPRITE_STATE_FADE_IN,
    FADE_BATTLER_SPRITE_STATE_HOLD,
    FADE_BATTLER_SPRITE_STATE_INIT_FADE_OUT,
    FADE_BATTLER_SPRITE_STATE_FADE_OUT,
    FADE_BATTLER_SPRITE_STATE_CLEANUP,
};

static void ov07_02223F5C(SysTask *task, void *param);

static void ov07_02223F5C(SysTask *task, void *param) {
    FadeBattlerSpriteContext *ctx = param;

    switch (ctx->state) {
    case FADE_BATTLER_SPRITE_STATE_INIT:
        Pokepic_StartPaletteFade(ctx->sprite, 0, ctx->alpha, ctx->fadeDelay, ctx->color);
        ctx->state++;
        break;
    case FADE_BATTLER_SPRITE_STATE_FADE_IN:
        if (Pokepic_ResumePaletteFade(ctx->sprite) == FALSE) {
            ctx->state++;
        }
        break;
    case FADE_BATTLER_SPRITE_STATE_HOLD:
        if (ctx->holdDelay == 0) {
            ctx->state++;
        } else {
            ctx->holdDelay--;
            break;
        }
    case FADE_BATTLER_SPRITE_STATE_INIT_FADE_OUT:
        Pokepic_StartPaletteFade(ctx->sprite, ctx->alpha, 0, ctx->fadeDelay, ctx->color);
        ctx->state++;
        break;
    case FADE_BATTLER_SPRITE_STATE_FADE_OUT:
        if (Pokepic_ResumePaletteFade(ctx->sprite) == FALSE) {
            ctx->endDelay--;

            if (ctx->endDelay <= 0) {
                ctx->state++;
            } else {
                ctx->state = 0;
            }
        }
        break;
    case FADE_BATTLER_SPRITE_STATE_CLEANUP:
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }
}

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

int ov07_0223197C(BattleAnimSystem *system, int battler);

void ov07_02224008(BattleAnimSystem *system);

void ov07_02224008(BattleAnimSystem *system) {
    FadeBattlerSpriteContext *ctx = ov07_022324D8(system, sizeof(FadeBattlerSpriteContext));
    ctx->battleAnimSys = system;
    ctx->sprite = 0L;

    int targetBattler;
    switch (ov07_0221C4A8(system, 0)) {
    case (1 << 1):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
        break;
    case (1 << 2):
        if (ov07_0221FAB0(system) == 1) {
            ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0223197C(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys)));
        }
        break;
    case (1 << 3):
        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
        break;
    case (1 << 4):
        if (ov07_0221FAB0(system) == 1) {
            ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0223197C(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)));
        }
        break;
    case ((1 << 11) | (1 << 1)): {
        targetBattler = 0xFF;

        for (int battler = 0; battler < 4; battler++) {
            int type = ov07_0221FA04(system, battler);
            if (type == 0 || type == 2) {
                targetBattler = battler;
                break;
            }
        }

        if (targetBattler == 0xFF) {
            targetBattler = 0;
        }

        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, targetBattler);
        break;
    }
    case ((1 << 11) | (1 << 2)): {
        targetBattler = 0xFF;

        for (int battler = 0; battler < 4; battler++) {
            int type = ov07_0221FA04(system, battler);
            if (type == 4) {
                targetBattler = battler;
                break;
            }
        }

        if (targetBattler == 0xFF) {
            targetBattler = 0;
        }

        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, targetBattler);
        break;
    }
    case ((1 << 11) | (1 << 3)): {
        targetBattler = 0xFF;

        for (int battler = 0; battler < 4; battler++) {
            int type = ov07_0221FA04(system, battler);
            if (type == 1 || type == 3) {
                targetBattler = battler;
                break;
            }
        }

        if (targetBattler == 0xFF) {
            targetBattler = 0;
        }

        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, targetBattler);
        break;
    }
    case ((1 << 11) | (1 << 4)): {
        targetBattler = 0xFF;

        for (int battler = 0; battler < 4; battler++) {
            int type = ov07_0221FA04(system, battler);
            if (type == 5) {
                targetBattler = battler;
                break;
            }
        }

        if (targetBattler == 0xFF) {
            targetBattler = 0;
        }

        ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, targetBattler);
        break;
    }
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    if (ctx->sprite == 0L) {
        Heap_Free(ctx);
        return;
    }

    ctx->fadeDelay = ov07_0221C4A8(system, 1);
    ctx->endDelay = ov07_0221C4A8(system, 2);
    ctx->color = ov07_0221C4A8(system, 3);
    ctx->alpha = ov07_0221C4A8(system, 4);
    ctx->holdDelay = ov07_0221C4A8(system, 5);

    ov07_0221C410(ctx->battleAnimSys, ov07_02223F5C, ctx);
}

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

BOOL ov07_02222558(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

void ov07_022226FC(ManagedSprite *sprite, s16 y, s16 height, fx32 scale, enum BattleAnimAnchorType anchor);

enum ScalePokemonSpriteState {
    SCALE_POKEMON_SPRITE_STATE_INIT_SCALE = 0,
    SCALE_POKEMON_SPRITE_STATE_SCALE,
    SCALE_POKEMON_SPRITE_STATE_INIT_RESTORE,
    SCALE_POKEMON_SPRITE_STATE_RESTORE,
    SCALE_POKEMON_SPRITE_STATE_CLEANUP
};

void ov07_022241D8(SysTask *task, void *param);

void ov07_022241D8(SysTask *task, void *param) {
    ScalePokemonSpriteContext *ctx = param;
    f32 scaleX, scaleY;
    BOOL active;

    switch (ctx->state) {
    case SCALE_POKEMON_SPRITE_STATE_INIT_SCALE:
        ov07_02222508(
            &ctx->scale,
            (s16)ctx->startScale,
            (s16)ctx->refScale,
            (s16)ctx->endScale,
            ((ctx->scaleFrames) >> 16));
        ov07_02222644(&ctx->scale, &scaleX, &scaleY);

        if (ctx->flip == 1) {
            scaleX = -scaleX;
        }

        ManagedSprite_SetAffineScale(ctx->sprite, scaleX, scaleY);
        ctx->state++;
        break;
    case SCALE_POKEMON_SPRITE_STATE_SCALE:
        active = ov07_02222558(&ctx->scale);
        ov07_02222644(&ctx->scale, &scaleX, &scaleY);

        if (ctx->flip == 1) {
            scaleX = -scaleX;
        }

        ManagedSprite_SetAffineScale(ctx->sprite, scaleX, scaleY);

        if (active) {
            ov07_022226FC(
                ctx->sprite,
                ctx->baseY,
                ctx->battlerHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->state++;
        }
        break;
    case SCALE_POKEMON_SPRITE_STATE_INIT_RESTORE:
        ov07_02222508(
            &ctx->scale,
            (s16)ctx->endScale,
            (s16)ctx->refScale,
            (s16)ctx->startScale,
            ((ctx->scaleFrames) & 0xFFFF));
        ctx->state++;
        break;
    case SCALE_POKEMON_SPRITE_STATE_RESTORE:
        active = ov07_02222558(&ctx->scale);
        ov07_02222644(&ctx->scale, &scaleX, &scaleY);

        if (ctx->flip == 1) {
            scaleX = -scaleX;
        }

        ManagedSprite_SetAffineScale(ctx->sprite, scaleX, scaleY);

        if (active) {
            ov07_022226FC(
                ctx->sprite,
                ctx->baseY,
                ctx->battlerHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->cycles--;

            if (ctx->cycles <= 0) {
                ctx->state++;
            } else {
                ctx->state = SCALE_POKEMON_SPRITE_STATE_INIT_SCALE;
            }
        }
        break;
    case SCALE_POKEMON_SPRITE_STATE_CLEANUP:
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        return;
    }

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}
