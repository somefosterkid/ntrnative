#include "global.h"

#include "sprite_system.h"
#include "overlay_07_battle_anim_funcs.h"

u16 ov07_0221C468(BattleAnimSystem *system);

typedef struct AlphaFadeContext {
    XYTransformContext lerp;
    int done;
} AlphaFadeContext;

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

typedef struct MementoContext {
    BattleAnimSystem *battleAnimSys;
    SpriteManager *pokemonSpriteManager;
    int state;
    int delay;
    int attackerHeight;
    int attackerY;
    ManagedSprite *attackerSprite;
    ManagedSprite *defenderSprite;
    ManagedSprite *attackerPartnerSprite;
    ManagedSprite *defenderPartnerSprite;
    XYTransformContext scale;
    XYTransformContext scaleX;
    XYTransformContext defenderPos;
    int spriteState;
    int spriteDelay;
    AlphaFadeContext alpha;
    PaletteData *paletteData;
    BOOL attackerFlipped;
    BOOL defenderFlipped;
} MementoContext;

enum MementoState {
    MEMENTO_STATE_INIT = 0,
    MEMENTO_STATE_FADE_AND_INIT_ATTACKER,
    MEMENTO_STATE_UPDATE_ATTACKER,
    MEMENTO_STATE_INIT_DEFENDER,
    MEMENTO_STATE_UPDATE_DEFENDER,
    MEMENTO_STATE_FADE,
    MEMENTO_STATE_CLEANUP,

    MEMENTO_ATTACKER_STATE_STRETCH = 0,
    MEMENTO_ATTACKER_STATE_SQUISH,
    MEMENTO_ATTACKER_STATE_FINISH,

    MEMENTO_DEFENDER_STATE_MOVE = 0,
    MEMENTO_DEFENDER_STATE_SCALE,
    MEMENTO_DEFENDER_STATE_FINISH,
};

extern const u16 ov07_02236800[6];

void ov07_0222F408(MementoContext *ctx, ManagedSprite *sprite);

void ov07_0222F434(MementoContext *ctx);

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
BOOL ov07_02222558(XYTransformContext *ctx);
BOOL ov07_0222260C(XYTransformContext *ctx);
void ov07_022226FC(ManagedSprite *sprite, s16 y, s16 height, fx32 scale, enum BattleAnimAnchorType anchor);
BOOL ov07_0222F4D4(MementoContext *ctx);

void ov07_0222F434(MementoContext *ctx) {
    ov07_02222590(
        &ctx->scale,
        10,
        10,
        10,
        20,
        10,
        8);
    ov07_02222508(
        &ctx->scaleX,
        10,
        10,
        1,
        4);

    ctx->spriteState = MEMENTO_ATTACKER_STATE_STRETCH;
    ctx->spriteDelay = 6;

    int battlerType = ov07_02231924(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, ov07_02236800[battlerType]);
    ManagedSprite_SetDrawFlag(ctx->attackerSprite, 1);

    f32 scaleX, scaleY;
    ov07_02222644(&ctx->scale, &scaleX, &scaleY);

    if (ctx->attackerFlipped == 1) {
        scaleX = -scaleX;
    }

    ManagedSprite_SetAffineScale(ctx->attackerSprite, scaleX, scaleY);
    ov07_0222F408(ctx, ctx->attackerSprite);
}

BOOL ov07_0222F4D4(MementoContext *ctx) {
    BOOL done = 0;
    f32 scaleX, scaleY;

    switch (ctx->spriteState) {
    case MEMENTO_ATTACKER_STATE_STRETCH:
        ov07_0222260C(&ctx->scale);
        ov07_02222644(&ctx->scale, &scaleX, &scaleY);
        if (ctx->attackerFlipped == 1) {
            scaleX = -scaleX;
        }

        ManagedSprite_SetAffineScale(ctx->attackerSprite, scaleX, scaleY);
        ov07_022226FC(
            ctx->attackerSprite,
            ctx->attackerY,
            ctx->attackerHeight,
            ctx->scale.data[XY_PARAM_CUR_Y],
            BATTLE_ANIM_ANCHOR_BOTTOM);

        ctx->spriteDelay--;
        if (ctx->spriteDelay < 0) {
            ctx->spriteState++;

            ov07_02222590(
                &ctx->scale,
                10,
                1,
                20,
                20,
                10,
                4);
        }
        break;
    case MEMENTO_ATTACKER_STATE_SQUISH:
        ov07_0222260C(&ctx->scale);
        if (ov07_02222558(&ctx->scaleX)) {
            ctx->scale.x = ctx->scaleX.x;

            ov07_02222644(&ctx->scale, &scaleX, &scaleY);
            if (ctx->attackerFlipped == 1) {
                scaleX = -scaleX;
            }

            ManagedSprite_SetAffineScale(ctx->attackerSprite, scaleX, scaleY);
            ov07_022226FC(
                ctx->attackerSprite,
                ctx->attackerY,
                ctx->attackerHeight,
                ctx->scale.data[XY_PARAM_CUR_Y],
                BATTLE_ANIM_ANCHOR_BOTTOM);
        } else {
            ctx->spriteState++;

            ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, 0);
            ManagedSprite_SetDrawFlag(ctx->attackerSprite, 0);

            done = 1;
        }
        break;
    case MEMENTO_ATTACKER_STATE_FINISH:
        done = 1;
        break;
    }

    return done;
}
