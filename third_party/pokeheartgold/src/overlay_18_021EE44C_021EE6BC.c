#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "wild_encounter.h"

s32 ov18_021EE71C(PokedexAppData *, s32);

s32 ov18_021EE75C(PokedexAppData *, s32, s32);

s32 ov18_021EE7DC(PokedexAppData *, s32, s32);

s32 ov18_021EE834(PokedexAppData *, s32, s32, s32);

void ov18_021EE6BC(PokedexAppData *pokedexApp, int species, int a2) {
    ov18_021EE71C(pokedexApp, 7);
    ov18_021EE75C(pokedexApp, species, 8);
    ov18_021EE7DC(pokedexApp, species, 9);
    ov18_021EE834(pokedexApp, species, a2, 0xA);
}

void ov18_021EE6EC(PokedexAppData *pokedexApp, int species, int a2) {
    ov18_021EE71C(pokedexApp, 0x51);
    ov18_021EE75C(pokedexApp, species, 0x52);
    ov18_021EE7DC(pokedexApp, species, 0x53);
    ov18_021EE834(pokedexApp, species, a2, 0x54);
}
