#include "global.h"

#include "error_handling.h"
#include "overlay_98.h"

SpriteSystem *ov98_0221E5C0(PokeathlonSpriteContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    return ctx->spriteSystem;
}

SpriteManager *ov98_0221E5D0(PokeathlonSpriteContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    return ctx->spriteManager;
}
