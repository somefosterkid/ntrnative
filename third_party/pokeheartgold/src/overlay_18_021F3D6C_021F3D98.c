#include "global.h"

#include "application/pokedex/pokedex_internal.h"

ManagedSprite *ov18_021F11EC(PokedexAppData *, ManagedSpriteTemplate *);

s32 ov18_021F1534(PokedexAppData *, u32, u32, s32);

s32 ov18_021F3CA8(PokedexAppData *, s32, void *, s32 *);

s32 ov18_021F3E24(PokedexAppData *);

s32 ov18_021F40E4(PokedexAppData *);

s32 ov18_021F4188(PokedexAppData *);

s32 ov18_021F69C0(PokedexAppData *, s32);

extern ManagedSpriteTemplate ov18_021FA610[8];

void ov18_021F3D98(PokedexAppData *pokedexApp) {
    u8 sp0[4];
    u32 var_r6;

    ov18_021F3E24(pokedexApp);
    for (var_r6 = 1; var_r6 <= 8; var_r6++) {
        pokedexApp->unk_0670[var_r6] = ov18_021F11EC(pokedexApp, &ov18_021FA610[var_r6 - 1]);
    }
    ov18_021F69C0(pokedexApp, 1);
    ov18_021F3CA8(pokedexApp, 0, &sp0[1], (s32 *)sp0);
    ov18_021F1534(pokedexApp, pokedexApp->curSpecies, *(u8 *)((u8 *)((s32 *)sp0) + 1), 1);
    ov18_021F40E4(pokedexApp);
    ov18_021F40A0(pokedexApp);
    ov18_021F4188(pokedexApp);
}
