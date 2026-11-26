#include "global.h"

#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
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


void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

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

s32 ov07_0222207C(void *, ManagedSprite *, s16, s16);
s32 ov07_022222B4(void *);
s32 ov07_02222748(void *, ManagedSprite *);
s32 ov07_0222DC54(void *arg0);

BOOL ov07_0222DBD8(ScaryFaceSpriteContext *ctx) {
    BOOL done = 0;

    switch (ctx->attackerScaleState) {
    case SCARY_FACE_SPRITE_ATTACKER_STATE_GROW:
        if (ov07_02222558(&ctx->attackerScale)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_Y, ctx->attackerScale.y);
        } else {
            ctx->attackerScaleState++;
            ov07_02222508(
                &ctx->attackerScale,
                15,
                10,
                10,
                12);
        }
        break;
    case SCARY_FACE_SPRITE_ATTACKER_STATE_SHRINK:
        if (ov07_02222558(&ctx->attackerScale)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_Y, ctx->attackerScale.y);
        } else {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_SCALE_Y, 0x100);
            ctx->attackerScaleState++;
        }
        break;
    case SCARY_FACE_SPRITE_ATTACKER_STATE_DONE:
        done = 1;
        break;
    }

    return done;
}

s32 ov07_0222DC54(void *arg0) {
    s32 v0;
    s32 v1;

    v0 = *((s32 *)&((ScaryFaceSpriteContext *)arg0)->faceState);
    v1 = 0;
    switch (v0) {
    case 0:
        ov07_02222748(&((ScaryFaceSpriteContext *)arg0)->faceScale, ((ScaryFaceSpriteContext *)arg0)->sprite);
        if (ov07_022222B4(&((ScaryFaceSpriteContext *)arg0)->facePos) != 0) {
            ov07_0222207C(&((ScaryFaceSpriteContext *)arg0)->facePos, ((ScaryFaceSpriteContext *)arg0)->sprite, ((ScaryFaceSpriteContext *)arg0)->baseX, ((ScaryFaceSpriteContext *)arg0)->baseY);
        } else {
            *((s32 *)&((ScaryFaceSpriteContext *)arg0)->faceState) = (s32)(*((s32 *)&((ScaryFaceSpriteContext *)arg0)->faceState) + 1);
            ov07_02222AC4(&((ScaryFaceSpriteContext *)arg0)->faceAlpha, 0x10, 0, 0xE, 0x10, 8);
        }
        break;
    case 1:
        if (ov07_02222AF4(&((ScaryFaceSpriteContext *)arg0)->faceAlpha) != 0) {
            ManagedSprite_SetDrawFlag(((ScaryFaceSpriteContext *)arg0)->sprite, 0);
            *((s32 *)&((ScaryFaceSpriteContext *)arg0)->faceState) = (s32)(*((s32 *)&((ScaryFaceSpriteContext *)arg0)->faceState) + 1);
        }
        break;
    case 2:
        v1 = 1;
        break;
    }
    return v1;
}
