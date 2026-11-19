#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"

void ov51_021E6200(s32 arg0);

void ov51_021E6200(s32 arg0) {
    NNSG2dPaletteData *ppPlttData;
    s32 v0;
    void *v1;
    void *ptr;

    ptr = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 0x3C, &ppPlttData, HEAP_ID_25);
    v0 = arg0 << 5;
    v1 = ppPlttData->pRawData;
    DC_FlushRange(v1 + v0, 0x20U);
    GXS_LoadBGPltt(v1 + v0, 0x80U, 0x20U);
    Heap_Free(ptr);
}
