#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_swagger_sprite_context.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"

struct BattleAnimSystem;

enum SwaggerSpriteState {
    SWAGGER_SPRITE_STATE_ANIMATE = 0,
    SWAGGER_SPRITE_STATE_CLEANUP,

    SWAGGER_SPRITE_POP_STATE_INIT_1 = 0,
    SWAGGER_SPRITE_POP_STATE_SCALE_1,
    SWAGGER_SPRITE_POP_STATE_DELAY,
    SWAGGER_SPRITE_POP_STATE_SCALE_2,
    SWAGGER_SPRITE_POP_STATE_DONE,
};

BOOL ov07_02223D28(SwaggerSpriteContext *ctx);

static void ov07_02223DC4(SysTask *task, void *param);

static void ov07_02223DC4(SysTask *task, void *param) {
    SwaggerSpriteContext *ctx = param;

    switch (ctx->state) {
    case SWAGGER_SPRITE_STATE_ANIMATE:
        if (ov07_02223D28(ctx)) {
            ctx->state++;
        }
        break;
    case SWAGGER_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
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


enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

void ov07_02223E08(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02223E08(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    SwaggerSpriteContext *ctx = ov07_022324D8(system, sizeof(SwaggerSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->sprite = sprite;

    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetDrawFlag(ctx->sprite, 0);

    ctx->delay = 4;
    ctx->dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    Pokepic *monSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));

    ctx->defenderX = Pokepic_GetAttr(monSprite, MON_SPRITE_X_CENTER);
    ctx->defenderY = Pokepic_GetAttr(monSprite, MON_SPRITE_Y_CENTER);

    ov07_0221C410(ctx->battleAnimSys, ov07_02223DC4, ctx);
}
