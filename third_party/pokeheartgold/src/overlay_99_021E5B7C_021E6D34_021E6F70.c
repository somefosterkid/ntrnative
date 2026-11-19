#include "global.h"

#include "pokeathlon/pokeathlon_save.h"

#include "error_handling.h"

u16 ov99_021E6F70(PokeathlonSave *arg0);

u16 ov99_021E6F70(PokeathlonSave *pokeathlonSave) {
    u16 v2;
    u8 v1;
    void *v0;

    v2 = 0;
    if (pokeathlonSave == NULL) {
        GF_AssertFail();
    }
    v0 = PokeathlonSave_GetUnkAEC(pokeathlonSave);
    v1 = 0;
    do {
        v2 += *(u16 *)(v0 + (v1 * 2));
        v1 += 1;
    } while ((u32)v1 < 0xAU);
    return v2;
}
