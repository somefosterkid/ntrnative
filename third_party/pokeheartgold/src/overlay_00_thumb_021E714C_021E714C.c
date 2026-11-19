#include "global.h"

#include "heap.h"
#include "ptr_align.h"
#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern void *_0221A680;

void ov00_021E714C(s32 arg0, enum HeapID arg1);

void ov00_021E714C(s32 arg0, enum HeapID arg1) {
    void *v0;

    if (arg0 != 0) {
        *((s32 *)&((NintendoWFCManager *)_0221A680)->secondaryHeapID) = 0xB;
        if (((NintendoWFCManager *)_0221A680)->heap2AllocPtr == NULL) {
            ((NintendoWFCManager *)_0221A680)->heap2AllocPtr = Heap_Alloc(arg1, 0xF020U);
            ((NintendoWFCManager *)_0221A680)->heap2 = NNS_FndCreateExpHeapEx(PTR_ALIGN_UP(((NintendoWFCManager *)_0221A680)->heap2AllocPtr, 0x20), 0xF000U, 0U);
            NNS_FndSetGroupIDForExpHeap(((NintendoWFCManager *)_0221A680)->heap2, 0x10U);
            ((NintendoWFCManager *)_0221A680)->heap2Size = NNS_FndGetTotalFreeSizeForExpHeap(((NintendoWFCManager *)_0221A680)->heap2);
        }
    } else {
        *((s32 *)&((NintendoWFCManager *)_0221A680)->secondaryHeapID) = (s32) *((s32 *)&((NintendoWFCManager *)_0221A680)->primaryHeapID);
        v0 = _0221A680;
        if (((NintendoWFCManager *)v0)->heap2AllocPtr != NULL) {
            if (*((s32 *)&((NintendoWFCManager *)v0)->heap2Size) != NNS_FndGetTotalFreeSizeForExpHeap(((NintendoWFCManager *)v0)->heap2)) {
                sub_02039AD8(1);
                return;
            }
            NNS_FndDestroyExpHeap(((NintendoWFCManager *)v0)->heap2);
            Heap_Free(((NintendoWFCManager *)_0221A680)->heap2AllocPtr);
            ((NintendoWFCManager *)_0221A680)->heap2AllocPtr = NULL;
        }
    }
}

void ov00_021E7220(s32 arg0);

void ov00_021E7220(s32 arg0) {
    *((s8 *)&((NintendoWFCManager *)_0221A680)->disconnectIfAlone) = (s8)arg0;
}

s32 ov00_021E6638(s32, void *, s32);

void ov00_021E7234(s32 arg0);

void ov00_021E7234(s32 arg0) {
    s32 v0;
    void *ptr;

    v0 = arg0 * 4;
    ptr = *(void **)((u8 *)(_0221A680 + v0) + 0xF80);
    if (ptr != NULL) {
        if (*(u8 *)((u8 *)(_0221A680 + arg0) + 0x10E1) == 1) {
            Heap_Free(ptr);
        } else {
            ov00_021E6638(0, ptr, 0);
        }
        *(s32 *)((u8 *)(_0221A680 + v0) + 0xF80) = 0;
    }
}
