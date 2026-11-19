#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "struct_defs/resdat_narc_structs.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_alpha_fade_context.h"
#include "pokepic.h"
#include "overlay_07_faint_attack_context.h"

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222558(XYTransformContext *ctx);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

typedef struct ConstrictVine {
    ManagedSprite *sprite;
    XYTransformContext scale;
    BOOL flip;
    int scaleCounter;
    int nextStartScale;
    int nextEndScale;
} ConstrictVine;

BOOL ov07_0222D590(ConstrictVine *vine);

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
BOOL ov07_0222283C(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);
typedef struct ConstrictSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    int counter;
    int vineIdx;
    Pokepic *defenderSprite;
    s16 defenderX;
    s16 defenderY;
    XYTransformContext shake;
    ConstrictVine vines[4];
    int unused;
} ConstrictSpriteContext;
enum ConstrictSpriteState {
    CONSTRICT_SPRITE_STATE_ANIMATE = 0,
    CONSTRICT_SPRITE_STATE_SHAKE,
    CONSTRICT_SPRITE_STATE_SCALE,
    CONSTRICT_SPRITE_STATE_CLEANUP,
};
void ov07_0222D56C(ConstrictVine *vine);
void ov07_0222D578(ConstrictVine *vine);
void ov07_0222D60C(SysTask *task, void *param);
ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);
void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);
void ov07_0222D4F4(ConstrictVine *vine, ManagedSprite *sprite, s16 x, s16 y, BOOL flip);
void ov07_0222D6E0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_022220B8(XYTransformContext *ctx, Pokepic *sprite, s16 cx, s16 cy);
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
BOOL ov07_022222B4(XYTransformContext *ctx);
void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);
BOOL ov07_022223CC(XYTransformContext *ctx, XYTransformContext *revs, ManagedSprite *sprite);
typedef struct BonemerangSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    Pokepic *attackerSprite;
    s16 attackerX;
    s16 attackerY;
    XYTransformContext attackerPos;
    s32 dirX;
    Pokepic *defenderSprite;
    s16 defenderX;
    s16 defenderY;
    ManagedSprite *sprite;
    XYTransformContext linear;
    XYTransformContext revs;
} BonemerangSpriteContext;
enum BonemerangSpriteState {
    BONEMERANG_SPRITE_STATE_THROW_1 = 0,
    BONEMERANG_SPRITE_STATE_THROW_2,
    BONEMERANG_SPRITE_STATE_BONE_ARC_1,
    BONEMERANG_SPRITE_STATE_BONE_ARC_2,
    BONEMERANG_SPRITE_STATE_CATCH_1,
    BONEMERANG_SPRITE_STATE_CATCH_2,
    BONEMERANG_SPRITE_STATE_CLEANUP,
};
void ov07_0222D7A4(SysTask *task, void *param);
void ov07_0222D974(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);
BOOL ov07_02222218(XYTransformContext *ctx, s32 cx, s32 cy, ManagedSprite *sprite);
enum FaintAttackState {
    FAINT_ATTACK_STATE_REVOLVE = 0,
    FAINT_ATTACK_STATE_WAIT,
    FAINT_ATTACK_STATE_FADE_IN,
    FAINT_ATTACK_STATE_CLEANUP,
};
void ov07_0222DA60(SysTask *task, void *param);

BOOL ov07_0222D590(ConstrictVine *vine) {
    BOOL scaleActive = ov07_02222558(&vine->scale);
    if (scaleActive == 1) {
        f32 scaleX, scaleY;
        ov07_02222644(&vine->scale, &scaleX, &scaleY);

        if (vine->flip) {
            ManagedSprite_SetAffineScale(vine->sprite, -scaleX, 1.0f);
        } else {
            ManagedSprite_SetAffineScale(vine->sprite, scaleX, 1.0f);
        }
    } else {
        if (vine->scaleCounter > 0) {
            vine->scaleCounter--;
            ov07_02222508(
                &vine->scale,
                (s16)vine->nextStartScale,
                10,
                (s16)vine->nextEndScale,
                8);
            int startScale = vine->nextStartScale;
            vine->nextStartScale = vine->nextEndScale;
            vine->nextEndScale = startScale;
        } else {
            return 0;
        }
    }

    return 1;
}

void ov07_0222D60C(SysTask *task, void *param) {
    ConstrictSpriteContext *ctx = param;
    int i;

    switch (ctx->state) {
    case CONSTRICT_SPRITE_STATE_ANIMATE:
        if (ctx->counter >= 4) {
            ctx->counter = 0;
            ov07_0222D578(&ctx->vines[ctx->vineIdx]);
            ctx->vineIdx++;

            if (ctx->vineIdx >= 4) {
                ctx->state++;
            }
        }

        ctx->counter++;
        break;
    case CONSTRICT_SPRITE_STATE_SHAKE:
        if (ov07_0222283C(&ctx->shake, ctx->defenderX, ctx->defenderY, ctx->defenderSprite) == 0) {
            ctx->state++;
        }
        break;
    case CONSTRICT_SPRITE_STATE_SCALE: {
        BOOL active;
        for (i = 0; i < 4; i++) {

            active = ov07_0222D590(ctx->vines + i);
        }

        if (active == 0) {
            ctx->state++;
        }
        break;
    }
    case CONSTRICT_SPRITE_STATE_CLEANUP:
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER, ctx->defenderX);
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER, ctx->defenderY);

        for (i = 0; i < 4; i++) {
            ov07_0222D56C(ctx->vines + i);
        }

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_0222D6E0(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;

    ConstrictSpriteContext *ctx = ov07_022324D8(system, sizeof(ConstrictSpriteContext));
    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;

    int defender = ov07_0221C470(system);

    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, defender);
    ctx->defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);

    ov07_022227A8(
        &ctx->shake,
        4,
        0,
        1,
        1);

    int vineY = ctx->defenderY + 16;
    BOOL flip = 0;
    template = ov07_0221F9E8(ctx->battleAnimSys);

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            ov07_0222D4F4(
                &ctx->vines[i],
                sprite,
                ctx->defenderX,
                vineY,
                flip);
        } else {
            ov07_0222D4F4(
                &ctx->vines[i],
                SpriteSystem_NewSprite(spriteSys, spriteMan, &template),
                ctx->defenderX,
                vineY,
                flip);
        }

        vineY -= 10;
        flip ^= 1;
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_0222D60C, ctx);
}

void ov07_0222D7A4(SysTask *task, void *param) {
    BonemerangSpriteContext *ctx = param;

    switch (ctx->state) {
    case BONEMERANG_SPRITE_STATE_THROW_1:
        if (ov07_022222B4(&ctx->attackerPos)) {
            ov07_022220B8(
                &ctx->attackerPos,
                ctx->attackerSprite,
                ctx->attackerX,
                ctx->attackerY);
        } else {
            ov07_02222268(
                &ctx->attackerPos,
                ctx->attackerPos.x,
                0,
                0,
                0,
                2);
            ctx->state++;
        }

        return;
    case BONEMERANG_SPRITE_STATE_THROW_2:
        if (ov07_022222B4(&ctx->attackerPos)) {
            ov07_022220B8(
                &ctx->attackerPos,
                ctx->attackerSprite,
                ctx->attackerX,
                ctx->attackerY);
        } else {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY);
            ov07_02222268(
                &ctx->attackerPos,
                0,
                (-32) * ctx->dirX,
                0,
                0,
                4);
            ctx->state++;
        }

        return;
    case BONEMERANG_SPRITE_STATE_BONE_ARC_1:
        if (ov07_022223CC(&ctx->linear, &ctx->revs, ctx->sprite) == 0) {
            ov07_02222338(
                &ctx->linear,
                &ctx->revs,
                ctx->defenderX,
                ctx->attackerX,
                ctx->defenderY,
                ctx->attackerY,
                10,
                -((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f))));
            ctx->state++;
        }
        break;
    case BONEMERANG_SPRITE_STATE_BONE_ARC_2:
        if (ov07_022223CC(&ctx->linear, &ctx->revs, ctx->sprite) == 0) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
            ctx->state++;
        }
        break;
    case BONEMERANG_SPRITE_STATE_CATCH_1:
        if (ov07_022222B4(&ctx->attackerPos)) {
            ov07_022220B8(
                &ctx->attackerPos,
                ctx->attackerSprite,
                ctx->attackerX,
                ctx->attackerY);
        } else {
            ov07_02222268(
                &ctx->attackerPos,
                ctx->attackerPos.x,
                0,
                0,
                0,
                4);
            ctx->state++;
        }
        break;
    case BONEMERANG_SPRITE_STATE_CATCH_2:
        if (ov07_022222B4(&ctx->attackerPos)) {
            ov07_022220B8(
                &ctx->attackerPos,
                ctx->attackerSprite,
                ctx->attackerX,
                ctx->attackerY);
        } else {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY);

            ctx->state++;
        }
        break;
    case BONEMERANG_SPRITE_STATE_CLEANUP:
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}

void ov07_0222D974(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    BonemerangSpriteContext *ctx = ov07_022324D8(system, sizeof(BonemerangSpriteContext));

    ctx->battleAnimSys = system;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->dirX = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);

    ov07_02222268(
        &ctx->attackerPos,
        0,
        (-16) * ctx->dirX,
        0,
        0,
        4);

    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));
    ctx->defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    ctx->defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);
    ctx->sprite = sprite;

    ManagedSprite_SetAnimateFlag(ctx->sprite, 1);
    ManagedSprite_SetDrawPriority(ctx->sprite, 100);
    ManagedSprite_SetPriority(ctx->sprite, 1);

    ov07_02222338(
        &ctx->linear,
        &ctx->revs,
        ctx->attackerX,
        ctx->defenderX,
        ctx->attackerY,
        ctx->defenderY,
        10,
        ((fx32)(((32) > 0) ? ((32) * (1 << 12) + 0.5f) : ((32) * (1 << 12) - 0.5f))));

    ov07_0221C410(ctx->battleAnimSys, ov07_0222D7A4, ctx);
}

void ov07_0222DA60(SysTask *task, void *param) {
    FaintAttackContext *ctx = param;

    switch (ctx->state) {
    case FAINT_ATTACK_STATE_REVOLVE:
        if (ov07_02222218(
                &ctx->revs,
                ctx->attackerX,
                (s16)(ctx->attackerY - ctx->attackerShadowHeight),
                ctx->sprite)
            == 0) {
            ctx->state++;
            ctx->delay = 16;
        }
        break;
    case FAINT_ATTACK_STATE_WAIT:
        ctx->delay--;

        if (ctx->delay < 0) {
            ctx->state++;

            ov07_02222AC4(
                &ctx->alpha,
                0,
                16,
                16 - 0,
                16 - 16,
                32);
        }
        break;
    case FAINT_ATTACK_STATE_FADE_IN:
        if (ov07_02222AF4(&ctx->alpha)) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 1);
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_HIDE, 0);
            ctx->state++;
        }
        break;
    case FAINT_ATTACK_STATE_CLEANUP:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}
