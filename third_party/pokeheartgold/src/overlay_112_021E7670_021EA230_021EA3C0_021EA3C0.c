#include "global.h"

#include "overlay_112_pokewalker_app.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov112_021EA984(Ov112PokewalkerApp *app);

extern u8 ov112_021FEC88[];

extern ManagedSpriteTemplate ov112_021FED1C[];

extern ManagedSpriteTemplate ov112_021FF028;

extern ManagedSpriteTemplate ov112_021FF05C;

void ov112_021EA3C0(Ov112PokewalkerApp *app);

void ov112_021EA3C0(Ov112PokewalkerApp *app) {
    u32 i;

    for (i = 0; i < 0x11; i++) {
        app->sprites[i] = SpriteSystem_NewSpriteWithYOffset(app->spriteSystem, app->spriteManager, &ov112_021FED1C[i], FX32_CONST(256));
#if defined(SDK_PORT) || defined(NONMATCHING)
        // ov112_021FEC88 has 16 bytes but this loop runs 0x11 times: on the DS the 17th read is the first byte of the table
        // that follows it in the same section (ov112_021FEC98, which starts with 0x11), so that is the value used here.
        ManagedSprite_SetPaletteOverride(app->sprites[i], i < 16 ? ov112_021FEC88[i] : 0x11);
#else
        ManagedSprite_SetPaletteOverride(app->sprites[i], ov112_021FEC88[i]);
#endif
        ManagedSprite_SetDrawFlag(app->sprites[i], 0);
    }
    ManagedSprite_SetPriority(app->sprites[1], 2);
    for (; i < 0x75; i++) {
        app->sprites[i] = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &ov112_021FF028);
        ManagedSprite_SetPaletteOverride(app->sprites[i], 0);
        ManagedSprite_SetDrawFlag(app->sprites[i], 0);
        ManagedSprite_SetPriority(app->sprites[i], 2);
    }
    for (; i < 0xD9; i++) {
        app->sprites[i] = SpriteSystem_NewSpriteWithYOffset(app->spriteSystem, app->spriteManager, &ov112_021FF05C, FX32_CONST(256));
        ManagedSprite_SetPaletteOverride(app->sprites[i], 0);
        ManagedSprite_SetDrawFlag(app->sprites[i], 0);
        ManagedSprite_SetPriority(app->sprites[i], 2);
    }
    ov112_021EA984(app);
    ManagedSprite_SetDrawPriority(app->sprites[8], 2);
    ManagedSprite_SetAffineOverwriteMode(app->sprites[8], 1);
    ManagedSprite_SetAnimateFlag(app->sprites[3], 1);
    ManagedSprite_SetAnimateFlag(app->sprites[4], 1);
}
