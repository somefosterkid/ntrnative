#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_grass_whistle_sprite_context.h"
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


enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_02230058(SysTask *task, void *param);

void ov07_02230094(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02230094(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    GrassWhistleSpriteContext *ctx = ov07_022324D8(system, sizeof(GrassWhistleSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;

    Pokepic *attacker = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    s16 attackerX = Pokepic_GetAttr(attacker, MON_SPRITE_X_CENTER);
    s16 attackerY = Pokepic_GetAttr(attacker, MON_SPRITE_Y_CENTER);

    ManagedSpriteTemplate template;
    template = ov07_0221F9E8(system);

    for (int i = 0; i < 15; i++) {
        if (i == 0) {
            ctx->sprites[i] = sprite;
        } else {
            ctx->sprites[i] = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &template);
        }

        ManagedSprite_SetDrawPriority(ctx->sprites[i], 100);
        ManagedSprite_SetPriority(ctx->sprites[i], 1);
        ManagedSprite_SetAffineOverwriteMode(ctx->sprites[i], AFFINE_OVERWRITE_MODE_DOUBLE);
        ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
        ManagedSprite_SetPositionXY(ctx->sprites[i], attackerX, attackerY);
        ManagedSprite_SetAnim(ctx->sprites[i], i % 3);
    }

    ctx->dirX = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->dirY = ov07_0222202C(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ov07_0221C410(ctx->battleAnimSys, ov07_02230058, ctx);
}
