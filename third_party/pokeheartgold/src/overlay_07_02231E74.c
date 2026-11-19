#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"

struct BattleAnimSystem;

int ov07_0221FAF8(BattleAnimSystem *system, int bg);

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
BOOL Pokepic_IsActive(Pokepic *monSprite);
void ov07_02231FA0(Pokepic *sprite, Point2D *pos);
void ov07_02231FD8(ManagedSprite *sprite, Point2D *pos);

void ov07_02231E74(BattleAnimSystem *system, int alphaBase, int alphaEffect) {
    if (alphaBase == -1) {
        alphaBase = 26;
    }

    if (alphaEffect == -1) {
        alphaEffect = 5;
    }

    G2_SetBlendAlpha(
        1 << ov07_0221FAF8(system, 1),
        (1 << ov07_0221FAF8(system, 2)) | GX_WND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ,
        alphaBase,
        alphaEffect);
}

enum BattleAnimWindow {
    BATTLE_ANIM_WINDOW_0 = 0,
    BATTLE_ANIM_WINDOW_1,
};

void ov07_02231EC0(BattleAnimSystem *system, enum BattleAnimWindow window, BOOL applyEffects);

void ov07_02231EC0(BattleAnimSystem *system, enum BattleAnimWindow window, BOOL applyEffects) {
    if (window == BATTLE_ANIM_WINDOW_0) {
        G2_SetWnd0InsidePlane(
            (1 << ov07_0221FAF8(system, 2)) | (1 << ov07_0221FAF8(system, 0)) | (1 << ov07_0221FAF8(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ,
            applyEffects);
    } else {
        G2_SetWnd1InsidePlane(
            (1 << ov07_0221FAF8(system, 2)) | (1 << ov07_0221FAF8(system, 0)) | (1 << ov07_0221FAF8(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ,
            applyEffects);
    }

    G2_SetWndOutsidePlane(
        (1 << ov07_0221FAF8(system, 0)) | (1 << ov07_0221FAF8(system, 1)) | GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_OBJ,
        1);
}

void ov07_02231FA0(Pokepic *sprite, Point2D *pos) {
    if (sprite == 0L) {
        pos->x = 0;
        pos->y = 0;
        return;
    } else if (Pokepic_IsActive(sprite) == 0) {
        pos->x = 0;
        pos->y = 0;
        return;
    }

    pos->x = Pokepic_GetAttr(sprite, MON_SPRITE_X_CENTER);
    pos->y = Pokepic_GetAttr(sprite, MON_SPRITE_Y_CENTER);
}

void ov07_02231FD8(ManagedSprite *sprite, Point2D *pos) {
    ManagedSprite_GetPositionXY(sprite, &pos->x, &pos->y);
}
