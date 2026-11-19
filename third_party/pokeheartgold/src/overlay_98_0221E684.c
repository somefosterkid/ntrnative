#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_98.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "error_handling.h"


void ov98_0221E684(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, u32 count) {
    u32 uVar1;

    NARC_Delete(ctx->narc);
    PaletteData_Free(ctx->paletteData);
    uVar1 = 0;
    while (uVar1 < count) {
        if (*sprites != NULL) {
            Sprite_DeleteAndFreeResources(*sprites);
            *sprites = NULL;
        }
        uVar1 = uVar1 + 1;
        sprites = sprites + 1;
    }
    SpriteSystem_FreeResourcesAndManager(ctx->spriteSystem, ctx->spriteManager);
    SpriteSystem_Free(ctx->spriteSystem);
    Heap_Free(ctx);
    return;
}

void ov98_0221E6CC(PokeathlonSpriteContext *ctx) {
    if (ctx == NULL) {
        GF_AssertFail();
    }
    SpriteSystem_DrawSprites(ctx->spriteManager);
}

ManagedSprite *ov98_0221E6E0(PokeathlonSpriteContext *ctx, ManagedSpriteTemplate *unkTemplate) {
    return SpriteSystem_NewSprite(ctx->spriteSystem, ctx->spriteManager, unkTemplate);
}
