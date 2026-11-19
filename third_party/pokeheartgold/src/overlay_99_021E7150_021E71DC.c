#include "global.h"
#include "overlay_99.h"
#include "overlay_99_pokeathlon_medals.h"
struct PokeathlonMedals;

s32 ov99_021E71DC(struct PokeathlonMedals *arg0);

s32 ov99_021E71DC(PokeathlonMedals *arg0) {
    return *(s32 *)&arg0->args->pokedex;
}
