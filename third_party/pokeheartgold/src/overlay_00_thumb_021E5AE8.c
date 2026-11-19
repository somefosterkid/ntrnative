#include "global.h"

#include "constants/heap.h"

#include "heap.h"

void ov00_021E7300(void);

void DWC_ClearError();

extern u8 *_0221A680;

void ov00_021E5AE8(void);

void ov00_021E5AE8(void) {
    if (_0221A680 != 0) {
        ov00_021E7300();
        if (*(s32 *)(_0221A680 + 0xf98) != 0) {
            NNS_FndDestroyExpHeap(*(NNSFndHeapHandle *)(_0221A680 + 0xf9c));
            Heap_Free(*(void **)(_0221A680 + 0xf98));
            *(u32 *)(_0221A680 + 0xf98) = 0;
        }
        NNS_FndDestroyExpHeap(*(NNSFndHeapHandle *)(_0221A680 + 0xf94));
        Heap_FreeExplicit(*(enum HeapID *)(_0221A680 + 0x1084), *(void **)(_0221A680 + 0xf90));
        Heap_FreeExplicit(*(enum HeapID *)(_0221A680 + 0x1084), *(void **)(_0221A680 + 0xf7c));
        _0221A680 = 0;
    }
    DWC_ClearError();
    return;
}
