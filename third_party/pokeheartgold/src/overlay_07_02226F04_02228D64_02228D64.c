#include "global.h"

#include "bg_window.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "pokepic.h"

struct BattleAnimSystem;

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

int ov07_0221FAF8(BattleAnimSystem *system, int bg);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

void ov07_02231E44(BattleAnimSystem *system, int planes, int spriteAlpha, int otherAlpha);

void ov07_02231FA0(Pokepic *sprite, Point2D *pos);

typedef struct NightmareContext {
    u8 state;
    u8 unused;
    u8 sLotoId;
    u8 unused2;
    Point2D monPos;
    BgConfig *bgConfig;
    BattleAnimSystem *system;
    SpriteManager *pokemonSpriteManager;
    Pokepic *monSprite;
    ManagedSprite *xluSprite;
    XYTransformContext shake;
    XYTransformContext pos;
    AlphaFadeContext alpha;
} NightmareContext;

void ov07_02228D08(SysTask *task, void *param);

void ov07_02228D64(BattleAnimSystem *system);

void ov07_02228D64(BattleAnimSystem *system) {
    NightmareContext *ctx = ov07_022324D8(system, sizeof(NightmareContext));

    ctx->state = 0;
    ctx->unused = 0;
    ctx->system = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->system);
    ctx->bgConfig = ov07_0221C4A0(ctx->system);
    ctx->xluSprite = ov07_0221C4E8(ctx->system, 0);

    int battler;
    if (ov07_0221C4A8(system, 0) == 0) {
        battler = ov07_0221C470(ctx->system);
    } else {
        battler = ov07_0221C468(ctx->system);
    }

    ctx->monSprite = ov07_0221FA48(ctx->system, battler);
    ov07_02231FA0(ctx->monSprite, &ctx->monPos);
    ctx->monPos.y -= Pokepic_GetAttr(ctx->monSprite, MON_SPRITE_SHADOW_HEIGHT);

    int dirX = ov07_02222004(ctx->system, battler);
    int dirY = ov07_0222202C(ctx->system, battler);

    ov07_02222268(
        &ctx->pos,
        ctx->monPos.x,
        ctx->monPos.x + (-20) * dirX,
        ctx->monPos.y,
        ctx->monPos.y + (20) * dirY,
        20);

    ov07_02231E44(
        ctx->system,
        (1 << ov07_0221FAF8(system, 2)) | GX_BLEND_PLANEMASK_BD | (1 << ov07_0221FAF8(system, 1)) | GX_WND_PLANEMASK_BG0,
        -1,
        -1);

    ov07_02222AC4(
        &ctx->alpha,
        31,
        0,
        0,
        31,
        (20 - 5));

    ManagedSprite_SetOamMode(ctx->xluSprite, GX_OAM_MODE_XLU);
    ov07_0221C410(ctx->system, ov07_02228D08, ctx);
}
