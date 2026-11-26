#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021EE35C(PokedexAppData *, s32 *, s32);

extern s32 ov18_021F9EBC;

void ov18_021F0900(PokedexAppData *pokedexApp) {
    ov18_021EE35C(pokedexApp, &ov18_021F9EBC, 0x10);
    ov18_021F0928(pokedexApp);
}

s32 ov18_021F0D7C(PokedexAppData *param_1);

void ov18_021F0918(PokedexAppData *pokedexApp) {
    ov18_021F0D7C(pokedexApp);
    ov18_021EE388(pokedexApp);
}

s32 ov18_021F0C50(PokedexAppData *);

void ov18_021F0928(PokedexAppData *pokedexApp) {
    ov18_021F0940(pokedexApp);
    ov18_021F0C50(pokedexApp);
    ov18_021F0D2C(pokedexApp);
}
