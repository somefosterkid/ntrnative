#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "system.h"

extern s8 _0210F930;

void sub_0203A4D4(enum NNS_G2D_VRAM_TYPE vramType, s32 unusedIsWifi, u32 offset, enum HeapID heapID);

void sub_0203A4D4(enum NNS_G2D_VRAM_TYPE vramType, s32 unusedIsWifi, u32 offset, enum HeapID heapID) {
    void *sp4;
    NNSG2dPaletteData *sp0;
    void *temp_r0;

    temp_r0 = Heap_AllocAtEnd(heapID, 0x258U);
    sp4 = temp_r0;
    if (temp_r0 != NULL) {
        sub_0201A3F8(&_0210F930, &sp4);
        DC_FlushRange(sp4, 0x258U);
        NNS_G2dGetUnpackedPaletteData(sp4, &sp0);
        if (vramType == NNS_G2D_VRAM_TYPE_2DMAIN) {
            GX_LoadOBJPltt(sp0->pRawData, offset, 0x20U);
        } else {
            GXS_LoadOBJPltt(sp0->pRawData, offset, 0x20U);
        }
        Heap_Free(sp4);
    }
}
