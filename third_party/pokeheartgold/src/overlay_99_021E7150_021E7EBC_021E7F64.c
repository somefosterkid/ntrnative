#include "global.h"
#include "overlay_99_pokeathlon_medals.h"
struct PokeathlonMedals;

void *ov98_0221E5C0(void *);

void *ov98_0221E5D0(void *);

s32 ov98_0221E784(void *, void *, s32 *, s32);

extern s32 ov99_021E9F70;

void ov99_021E7F64(struct PokeathlonMedals *arg0);

void ov99_021E7F64(PokeathlonMedals *arg0) {
    void *v0;

    v0 = ov98_0221E5C0(*((void **)&arg0->spriteContext));
    ov98_0221E784(v0, ov98_0221E5D0(*((void **)&arg0->spriteContext)), &ov99_021E9F70, 2);
}
