#include "global.h"

#include "heap.h"
#include "unk_0200A090.h"

void ov90_02258DD0(GF_2DGfxResMan **arg0, s32 arg1, enum HeapID arg2);

void ov90_02258DD0(GF_2DGfxResMan **arg0, s32 num, enum HeapID heapID) {
    GF_2DGfxResMan **v0;
    GF_2DGfxResMan *v1;
    GfGfxResType type;
    GF_2DGfxResMan **v3;
    u32 size;
    void *v2;
    v0 = arg0;
    type = GF_GFX_RES_TYPE_CHAR;
    v3 = arg0;
    do {
        v1 = Create2DGfxResObjMan(num, type, heapID);
        type += 1;
        *v3 = v1;
        v3 += 1;
    } while (type < 4);
    size = num * 0x38;
    v2 = Heap_Alloc(heapID, size);
    *((void **)(((u8 *)v0) + 0x10)) = v2;
    memset(v2, 0, size);
    *((s32 *)(((u8 *)v0) + 0x14)) = num;
}
