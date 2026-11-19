#include "global.h"

#include "gf_gfx_loader.h"

extern const u32 ov70_02245AAC[];

void *ov70_0223F658(enum HeapID heapId, int index, u32 *sizeOut);

void *ov70_0223F658(enum HeapID heapId, int index, u32 *sizeOut) {
    void *ret;
    u32 size;

    ret = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, ov70_02245AAC[index], FALSE, heapId, TRUE, &size);
    *sizeOut = size / 2;
    return ret;
}
