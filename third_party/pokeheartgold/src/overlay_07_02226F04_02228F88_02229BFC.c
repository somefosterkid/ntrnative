#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
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

BOOL ov07_0221BFC0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

int ov07_0221FAE8(BattleAnimSystem *system);

int ov07_0221FAF8(BattleAnimSystem *system, int bg);

void ov07_02231E44(BattleAnimSystem *system, int planes, int spriteAlpha, int otherAlpha);

typedef struct TrickCupSprite {
    ManagedSprite *sprite;
    XYTransformContext arc[2];
} TrickCupSprite;

typedef struct TrickSpriteContext {
    u8 state;
    u8 delay;
    int cupAlpha;
    int bgAlpha;
    u8 iteration;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    Pokepic *attackerSprite;
    s16 attackerY;
    int attackerOffset;
    u16 unused;
    fx32 radius;
    TrickCupSprite cupSprites[2];
    s16 mx;
    s16 my;
    XYTransformContext attackerScale;
} TrickSpriteContext;

void ov07_022297B8(SysTask *task, void *param);

void ov07_02229BFC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02229BFC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    int i;
    ManagedSpriteTemplate template;
    TrickSpriteContext *ctx = ov07_022324D8(system, sizeof(TrickSpriteContext));

    ctx->state = 0;
    ctx->delay = 0;
    ctx->battleAnimSys = system;
    ctx->spriteMan = spriteMan;
    ctx->spriteSys = spriteSys;
    ctx->cupAlpha = 0;
    ctx->bgAlpha = 16;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);
    ctx->attackerOffset = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ov07_02231E44(ctx->battleAnimSys,
        1 << ov07_0221FAF8(system, 2)
            | 1 << ov07_0221FAF8(system, 1)
            | GX_WND_PLANEMASK_BG0,
        -1,
        -1);
    G2_ChangeBlendAlpha(ctx->cupAlpha, ctx->bgAlpha);

    template = ov07_0221F9E8(system);
    ctx->cupSprites[0].sprite = sprite;

    for (i = 1; i < ov07_0221C4A8(ctx->battleAnimSys, 0); i++) {
        ctx->cupSprites[i].sprite = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &template);
    }

    for (i = 0; i < ov07_0221C4A8(ctx->battleAnimSys, 0); i++) {
        ManagedSprite_SetOamMode(ctx->cupSprites[i].sprite, GX_OAM_MODE_XLU);
        ManagedSprite_SetPriority(ctx->cupSprites[i].sprite, ov07_0221FAE8(ctx->battleAnimSys) + 1);
    }

    if (ov07_0221BFC0(system) == 1) {
        ManagedSprite_SetPositionXY(ctx->cupSprites[0].sprite, 180 - 20, 54);
        ManagedSprite_SetPositionXY(ctx->cupSprites[1].sprite, 100 - 20, 39);
    } else {
        ManagedSprite_SetPositionXY(ctx->cupSprites[0].sprite, 100, 54);
        ManagedSprite_SetPositionXY(ctx->cupSprites[1].sprite, 180, 39);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_022297B8, ctx);
}
