#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"

u16 *ov70_0223F634(enum HeapID heapID, s32 unused, s32 *pokedexLength);

u16 *ov70_0223F634(enum HeapID heapID, s32 unused, s32 *pokedexLength) {
    u32 sizeOut;
    u16 *v0;

    v0 = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, 0xD, 0, heapID, 0, &sizeOut);
    *pokedexLength = (s32)(sizeOut >> 1);
    return v0;
}
