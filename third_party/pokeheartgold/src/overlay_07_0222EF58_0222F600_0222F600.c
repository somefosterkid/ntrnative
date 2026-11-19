#include "global.h"

#include "sprite_system.h"
#include "overlay_07_battle_anim_funcs.h"

u16 ov07_0221C470(BattleAnimSystem *system);

typedef struct AlphaFadeContext {
    XYTransformContext lerp;
    int done;
} AlphaFadeContext;

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222F0(XYTransformContext *ctx, ManagedSprite *sprite);

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

void ov07_0222F600(MementoContext *ctx);

void ov07_0222F600(MementoContext *ctx) {
    s16 posX, posY;
    ManagedSprite_GetPositionXY(ctx->defenderSprite, &posX, &posY);

    ov07_02222268(
        &ctx->defenderPos,
        posX,
        posX,
        posY - 64,
        posY,
        8);
    ov07_02222590(
        &ctx->scale,
        2,
        10,
        20,
        10,
        10,
        8);

    f32 scaleX, scaleY;
    ov07_02222644(&ctx->scale, &scaleX, &scaleY);
    if (ctx->defenderFlipped == 1) {
        scaleX = -scaleX;
    }

    ManagedSprite_SetAffineScale(ctx->defenderSprite, scaleX, scaleY);
    ov07_022222F0(&ctx->defenderPos, ctx->defenderSprite);

    ctx->spriteState = MEMENTO_DEFENDER_STATE_MOVE;
    ctx->spriteDelay = 6;

    int battlerType = ov07_02231924(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys));
    ManagedSprite_SetDrawFlag(ctx->defenderPartnerSprite, ov07_02236800[battlerType]);
    ManagedSprite_SetDrawFlag(ctx->defenderSprite, 1);

    ov07_0222F408(ctx, ctx->defenderSprite);
}
