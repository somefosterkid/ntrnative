#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_alpha_fade_sprite_context.h"
#include "pokepic.h"
#include "overlay_07_blink_attacker_context.h"

struct BattleAnimSystem;

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_022247F4(BattleAnimSystem *system) {
    AlphaFadePokemonSpriteContext *ctx = ov07_022324D8(system, sizeof(AlphaFadePokemonSpriteContext));

    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);

    int targets = ov07_0221C4A8(system, 0);

    for (int i = 0; i < 4; i++) {
        if (targets & (1 << (i))) {
            ManagedSprite *sprite = ov07_0221C4E8(ctx->battleAnimSys, i);
            ManagedSprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
        }
    }

    int spriteStartAlpha = ov07_0221C4A8(system, 1);
    int bgStartAlpha = ov07_0221C4A8(system, 3);

    ov07_02231E08(ctx->battleAnimSys, spriteStartAlpha, bgStartAlpha);
    ov07_02222AC4(
        &ctx->alpha,
        spriteStartAlpha,
        ov07_0221C4A8(system, 2),
        bgStartAlpha,
        ov07_0221C4A8(system, 4),
        ov07_0221C4A8(system, 5));

    ov07_0221C410(ctx->battleAnimSys, ov07_022247C8, ctx);
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

BOOL ov07_0221FAC8(BattleAnimSystem *system, int battler);

void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);

void ov07_022248A0(BattleAnimSystem *system);

void ov07_022248A0(BattleAnimSystem *system) {
    int i;
    BOOL hide = ov07_0221C4A8(system, 1);

    int spriteCount;
    BattleAnimSpriteInfo spriteInfo;
    ov07_02232020(
        system,
        ov07_0221C4A8(system, 0),
        &spriteInfo,
        &spriteCount);

    for (i = 0; i < spriteCount; i++) {
        if (ov07_0221FAC8(system, spriteInfo.battler) == 1 && hide == 0) {
            continue;
        }

        Pokepic_SetAttr(spriteInfo.monSprite, MON_SPRITE_HIDE, hide);
    }
}

void ov07_022248F4(SysTask *task, void *param);

void ov07_022248F4(SysTask *task, void *param) {
    BlinkAttackerContext *ctx = param;

    if (ctx->timer >= ctx->interval) {
        if (ctx->count == 0) {
            Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_HIDE, FALSE);
            Heap_Free(ctx);
            ov07_0221C448(ctx->battleAnimSys, task);
        } else {
            BOOL hide = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_HIDE);
            Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_HIDE, hide ^ 1);
            ctx->count--;
            ctx->timer = 0;
        }
    } else {
        ctx->timer++;
    }
}
