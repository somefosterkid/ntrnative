#include "global.h"

#include "application/pokedex/pokedex_internal.h"

ManagedSprite *ov18_021F11EC(PokedexAppData *, ManagedSpriteTemplate *);

s32 ov18_021F4A6C(void *arg0);

s32 ov18_021F4D64(PokedexAppData *);

s32 ov18_021F4DDC(PokedexAppData *);

s32 ov18_021F4E28(PokedexAppData *);

extern ManagedSpriteTemplate ov18_021FA7B0[9];

void ov18_021F49F8(PokedexAppData *pokedexApp) {
    u32 i;
    ov18_021F4A6C(pokedexApp);
    for (i = 1; i < 10; i++) {
        pokedexApp->unk_0670[i] = ov18_021F11EC(pokedexApp, &ov18_021FA7B0[i - 1]);
    }
    ov18_021F4D64(pokedexApp);
    ov18_021F4DDC(pokedexApp);
    ov18_021F4E28(pokedexApp);
    ov18_021F11C0(pokedexApp, 3, 0);
    ov18_021F11C0(pokedexApp, 5, 0);
}
