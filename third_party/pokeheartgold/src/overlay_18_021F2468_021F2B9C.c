#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s16 ov18_021F2B9C(PokedexAppData *pokedexApp) {
    if (pokedexApp->unk_1858 == 0) {
        return 0x90;
    }
    return 0x70;
}
