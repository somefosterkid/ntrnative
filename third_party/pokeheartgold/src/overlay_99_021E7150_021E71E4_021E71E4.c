#include "global.h"

#include "pokedex.h"

s32 ov99_021E71DC(void *arg0);

s32 ov99_021E71E4(void *arg0, u32 arg1);

s32 ov99_021E71E4(void *arg0, u32 species) {
    if ((species == 0) || (species > 0x1EDU)) {
        return 0;
    }
    return Pokedex_GetSeenFormByIdx((Pokedex *)ov99_021E71DC(arg0), (s32)species, 0);
}
