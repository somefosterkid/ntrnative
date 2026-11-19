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

typedef struct GrassWhistleSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    int unused;
    int dirX;
    int dirY;
    ManagedSprite *sprites[15];
    XYTransformContext notePos[15];
    int noteDelay;
    int noteIndex;
} GrassWhistleSpriteContext;

void ov07_0222FEB0(ManagedSprite *sprite, XYTransformContext *pos, int dirX, int dirY);

BOOL ov07_0222FF04(ManagedSprite *sprite, XYTransformContext *pos);

BOOL ov07_0222FFD0(GrassWhistleSpriteContext *ctx);

BOOL ov07_0222FFD0(GrassWhistleSpriteContext *ctx) {

    if (ctx->noteIndex < 15) {
        ctx->noteDelay++;

        if (ctx->noteDelay > 6) {
            ctx->noteDelay = 0;

            ov07_0222FEB0(ctx->sprites[ctx->noteIndex], &ctx->notePos[ctx->noteIndex], ctx->dirX, ctx->dirY);
            ctx->noteIndex++;
        }
    }

    BOOL done;
    for (int i = 0; i < ctx->noteIndex; i++) {
        done = ov07_0222FF04(ctx->sprites[i], &ctx->notePos[i]);
    }

    if (ctx->noteIndex >= 15) {
        if (done == 1) {
            return 1;
        }
    }

    return 0;
}
