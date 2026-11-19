#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_42.h"

UnkStruct_ov42_02227F68 **ov49_02258958(enum HeapID arg0);

UnkStruct_ov42_02227F68 **ov49_02258958(enum HeapID heapID) {
    UnkStruct_ov42_02227F68 **v1;
    void *v0;

    v1 = Heap_Alloc(heapID, 4U);
    *v1 = ov42_02227EE0(0x23U, 0x2AU, heapID);
    v0 = GfGfxLoader_LoadFromNarc(NARC_a_2_0_0, 0, 0, heapID, 1);
    ov42_02227F48(*v1, v0);
    Heap_Free(v0);
    return v1;
}
