#include "global.h"

#include "pokedex.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_medals.h"

Pokedex *ov99_021E71DC(void *);

s32 ov99_021E7208(void *arg0, u8 arg1);

void ov99_021E723C(void *arg0);
s32 ov98_0221EF80(s32 param_1);
s32 ov99_021E718C();
void ov99_021E724C(void);

u8 ov99_021E7180();
void ov99_021E7258(void *arg0);

s32 ov99_021E7208(void *arg0, u8 species) {
    s32 v0;

    if ((species == 0) || (species > 0x1EDU)) {
        return 0;
    }
    v0 = Pokedex_SpeciesGetLastSeenGender(ov99_021E71DC(arg0), (u16)species, 0U);
    if (v0 == -1) {
        v0 = (s32)GetGenderBySpeciesAndPersonality((u16)species, 0U);
    }
    return v0;
}

void ov99_021E723C(void *arg0) {
    Pokedex_GetSeenSpindaPersonality(ov99_021E71DC(arg0), 0U);
}

void ov99_021E724C(void) {
    ov98_0221EF80(ov99_021E718C());
}

void ov99_021E7258(void *arg0) {
    u8 v0;
    u32 v1;

    v1 = ov99_021E7180();
    v0 = 0;
    do {
        ManagedSprite_SetDrawFlag(((PokeathlonMedals *)arg0)->sprites[v0 + 15], v1 & 1);
        v1 = (u32)(v1 << 0x17) >> 0x18;
        v0 += 1;
    } while ((u32)v0 < 5U);
}
