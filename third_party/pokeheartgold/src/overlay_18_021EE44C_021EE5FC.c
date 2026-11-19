#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021EE5E4(PokedexAppData *, s32, u32);

void ov18_021EE5FC(PokedexAppData *pokedexApp) {
    ov18_021EE5E4(pokedexApp, pokedexApp->unk_185D + 2, pokedexApp->unk_0878.unk_7B4);
    ov18_021EE5E4(pokedexApp, pokedexApp->unk_185D + 4, pokedexApp->unk_0878.unk_7B6);
    pokedexApp->unk_185D ^= 1;
}
