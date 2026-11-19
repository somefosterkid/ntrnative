#include "global.h"
#include "overlay_99.h"
#include "overlay_99_pokeathlon_medals.h"
struct PokeathlonMedals;

u8 ov99_021E7180(struct PokeathlonMedals *arg0, s32 arg1);

u8 ov99_021E7180(PokeathlonMedals *arg0, s32 arg1) {
    return *(u8 *)(*(s32 *)&arg0->args->unk4 + (arg1 - 1));
}
