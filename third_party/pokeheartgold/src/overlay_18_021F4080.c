#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

s32 ov18_021F61DC(PokedexAppData *, s32, s32, s32 *, s32);
extern s32 ov18_021FA35A;

void ov18_021F4080(PokedexAppData *pokedexApp) {
    ov18_021F1294(pokedexApp, 4, 0x20, (s16)((pokedexApp->unk_18C9 << 5) + 0x4C), 1);
}

void ov18_021F40A0(PokedexAppData *pokedexApp) {
    ov18_021F61DC(pokedexApp, 5, (s8)pokedexApp->unk_18DC.unk_20.nMaps, &ov18_021FA35A, 9);
    ov18_021F65EC(pokedexApp);
    ov18_021F619C(pokedexApp, pokedexApp->unk_18CA, (s8)pokedexApp->unk_18DC.unk_20.nMaps, 6);
}
