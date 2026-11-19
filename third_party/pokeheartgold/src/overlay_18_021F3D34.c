#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

s32 ov18_021F2648(PokedexAppData *arg0);

extern ManagedSpriteTemplate ov18_021FA554;

s32 ov18_021F10E8(PokedexAppData *, s32);
s32 ov18_021F26E4(PokedexAppData *);
s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

void ov18_021F3D34(PokedexAppData *pokedexApp) {
    ov18_021F2648(pokedexApp);
    pokedexApp->unk_0670[0] = SpriteSystem_NewSprite(pokedexApp->unk_0668, pokedexApp->unk_066C, &ov18_021FA554);
    ManagedSprite_SetPaletteOverride(pokedexApp->unk_0670[0], 4);
    ov18_021F11C0(pokedexApp, 0, 0);
}

void ov18_021F3D6C(PokedexAppData *pokedexApp) {
    ov18_021F10E8(pokedexApp, 0);
    ov18_021F26E4(pokedexApp);
}

void ov18_021F3D80(PokedexAppData *pokedexApp, int a1) {
    ov18_021F1294(pokedexApp, 0, (s16)((a1 << 6) + 0x20), 0xB0, 0);
}
