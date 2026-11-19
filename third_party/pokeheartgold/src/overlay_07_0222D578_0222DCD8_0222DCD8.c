#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "pokepic.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct ScaryFaceSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    Pokepic *attackerSprite;
    XYTransformContext attackerScale;
    int attackerScaleState;
    ManagedSprite *sprite;
    int faceState;
    XYTransformContext faceScale;
    XYTransformContext facePos;
    AlphaFadeContext faceAlpha;
    s16 baseX;
    s16 baseY;
} ScaryFaceSpriteContext;

enum ScaryFaceSpriteState {
    SCARY_FACE_SPRITE_STATE_ANIMATE = 0,
    SCARY_FACE_SPRITE_STATE_CLEANUP,

    SCARY_FACE_SPRITE_ATTACKER_STATE_GROW = 0,
    SCARY_FACE_SPRITE_ATTACKER_STATE_SHRINK,
    SCARY_FACE_SPRITE_ATTACKER_STATE_DONE,

    SCARY_FACE_SPRITE_FACE_STATE_GROW_AND_MOVE = 0,
    SCARY_FACE_SPRITE_FACE_STATE_FADE_OUT,
    SCARY_FACE_SPRITE_FACE_STATE_DONE,
};

BOOL ov07_0222DBD8(ScaryFaceSpriteContext *ctx);

BOOL ov07_0222DC54(ScaryFaceSpriteContext *ctx);

static void ov07_0222DCD8(SysTask *task, void *param);

static void ov07_0222DCD8(SysTask *task, void *param) {
    ScaryFaceSpriteContext *ctx = param;

    switch (ctx->state) {
    case SCARY_FACE_SPRITE_STATE_ANIMATE:
        ov07_0222DBD8(ctx);
        if (ov07_0222DC54(ctx)) {
            ctx->state++;
        }
        break;
    case SCARY_FACE_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

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


enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

enum {
    GX_BLEND_BGALL = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3,
    GX_BLEND_ALL = GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
};

void ov07_0222DD20(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222DD20(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ScaryFaceSpriteContext *ctx = ov07_022324D8(system, sizeof(ScaryFaceSpriteContext));
    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;

    int dirX = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    int dirY = ov07_0222202C(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));

    s16 attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    s16 attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);

    ov07_02222508(
        &ctx->attackerScale,
        10,
        10,
        15,
        12);
    ctx->sprite = sprite;

    ManagedSprite_SetAnimateFlag(ctx->sprite, 1);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprite, AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetOamMode(ctx->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);

    ctx->baseX = attackerX + (32 * dirX);
    ctx->baseY = attackerY;

    int moveY;
    if (dirY < 0) {
        moveY = (-8);
    } else {
        moveY = (-24);
    }

    ov07_02222268(&ctx->facePos, 0, 0 * dirX, 0, moveY * dirY, 32);
    ov07_02222508(
        &ctx->faceScale,
        5,
        10,
        12,
        32);

    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_ALL, 31, 26);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222DCD8, ctx);
}
