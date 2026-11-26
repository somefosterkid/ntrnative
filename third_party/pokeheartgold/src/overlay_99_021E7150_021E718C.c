#include "global.h"
#include "overlay_98.h"
#include "overlay_99.h"
#include "overlay_99_pokeathlon_medals.h"
struct PokeathlonMedals;


void ov99_021E718C(struct PokeathlonMedals *arg0);

void ov99_021E718C(PokeathlonMedals *arg0) {
    ov98_0221F024(arg0->args->unk4);
}

u8 ov99_021E7180(PokeathlonMedals *arg0, s32 arg1);

s32 ov99_021E7198(PokeathlonMedals *arg0, s32 arg1);

s32 ov99_021E7198(PokeathlonMedals *arg0, s32 arg1) {
    if (MATH_CountPopulation(ov99_021E7180(arg0, arg1)) == 5) {
        return 1;
    }
    return 0;
}
