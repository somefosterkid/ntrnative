#include "global.h"
struct PokeathlonMedals;

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_99_pokeathlon_medals.h"

s32 ov99_021E714C(void *);

extern const u32 ov99_021E9F4C[];

void *ov99_021E728C(struct PokeathlonMedals *param_1, u32 *param_2);

void *ov99_021E728C(PokeathlonMedals *param_1, u32 *param_2) {
    u32 size;
    void *ret;
    ret = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(param_1->zukanNarc, ov99_021E9F4C[ov99_021E714C(param_1)], FALSE, param_1->heapId, FALSE, &size);
    *param_2 = size / 2;
    return ret;
}
