#include "global.h"

#include "sprite.h"
#include "sprite_system.h"

struct BattleAnimSystem;

typedef struct BattleAnimSystem BattleAnimSystem;

typedef struct XYTransformContext {
    s16 x;
    s16 y;
    s32 data[8];
} XYTransformContext;

void ov07_02222508(XYTransformContext *ctx, s32 startScale, s32 refScale, s32 endScale, u32 steps);

BOOL ov07_02222748(XYTransformContext *ctx, ManagedSprite *sprite);

typedef struct SwaggerSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int state;
    int unused;
    ManagedSprite *sprite;
    XYTransformContext scale;
    int dir;
    int scaleStep;
    int delay;
    int popState;
    s16 defenderX;
    s16 defenderY;
} SwaggerSpriteContext;

BOOL ov07_02223CE4(SwaggerSpriteContext *ctx);

BOOL ov07_02223CE4(SwaggerSpriteContext *ctx) {
    if (ov07_02222748(&ctx->scale, ctx->sprite) == 0) {
        if (ctx->scaleStep) {
            ManagedSprite_SetDrawFlag(ctx->sprite, 0);
            return 1;
        } else {
            ctx->scaleStep = 1;
            ov07_02222508(
                &ctx->scale,
                14,
                10,
                12,
                2);
        }
    }

    return 0;
}
