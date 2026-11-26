#include "global.h"

#include "heap.h"
#include "ptr_align.h"
#include "unk_02037C94.h"
#include "overlay_00_nintendo_wfcmanager.h"

extern NintendoWFCManager *_0221A680;

void ov00_021E714C(s32 arg0, enum HeapID arg1);

void ov00_021E714C(s32 arg0, enum HeapID arg1) {
    NintendoWFCManager *v0;

    if (arg0 != 0) {
        *((s32 *)&_0221A680->secondaryHeapID) = 0xB;
        if (_0221A680->heap2AllocPtr == NULL) {
            _0221A680->heap2AllocPtr = Heap_Alloc(arg1, 0xF020U);
            _0221A680->heap2 = NNS_FndCreateExpHeapEx(PTR_ALIGN_UP(_0221A680->heap2AllocPtr, 0x20), 0xF000U, 0U);
            NNS_FndSetGroupIDForExpHeap(_0221A680->heap2, 0x10U);
            _0221A680->heap2Size = NNS_FndGetTotalFreeSizeForExpHeap(_0221A680->heap2);
        }
    } else {
        *((s32 *)&_0221A680->secondaryHeapID) = (s32) *((s32 *)&_0221A680->primaryHeapID);
        v0 = _0221A680;
        if (v0->heap2AllocPtr != NULL) {
            if (*((s32 *)&v0->heap2Size) != NNS_FndGetTotalFreeSizeForExpHeap(v0->heap2)) {
                sub_02039AD8(1);
                return;
            }
            NNS_FndDestroyExpHeap(v0->heap2);
            Heap_Free(_0221A680->heap2AllocPtr);
            _0221A680->heap2AllocPtr = NULL;
        }
    }
}

void ov00_021E7220(s32 arg0);

void ov00_021E7220(s32 arg0) {
    *((s8 *)&_0221A680->disconnectIfAlone) = (s8)arg0;
}

s32 ov00_021E6638(s32, void *, s32);

void ov00_021E7234(s32 arg0);

void ov00_021E7234(s32 arg0) {
    void *ptr = _0221A680->receiveBuffers[arg0];
    if (ptr != NULL) {
        if (_0221A680->receiveBuffersOnGlobalHeap[arg0] == 1) {
            Heap_Free(ptr);
        } else {
            ov00_021E6638(0, ptr, 0);
        }
        _0221A680->receiveBuffers[arg0] = NULL;
    }
}
