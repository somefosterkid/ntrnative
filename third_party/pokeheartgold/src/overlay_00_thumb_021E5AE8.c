#include "global.h"

#include "overlay_00_nintendo_wfcmanager.h"

#include "constants/heap.h"

#include "heap.h"

void ov00_021E7300(void);

extern NintendoWFCManager *_0221A680;

void ov00_021E5AE8(void);

void ov00_021E5AE8(void) {
    if (_0221A680 != 0) {
        ov00_021E7300();
        if (_0221A680->heap2AllocPtr != NULL) {
            NNS_FndDestroyExpHeap(_0221A680->heap2);
            Heap_Free(_0221A680->heap2AllocPtr);
            _0221A680->heap2AllocPtr = NULL;
        }
        NNS_FndDestroyExpHeap(_0221A680->heap1);
        Heap_FreeExplicit((enum HeapID)_0221A680->primaryHeapID, _0221A680->heap1AllocPtr);
        Heap_FreeExplicit((enum HeapID)_0221A680->primaryHeapID, _0221A680->allocStartPtr);
        _0221A680 = NULL;
    }
    DWC_ClearError();
    return;
}
