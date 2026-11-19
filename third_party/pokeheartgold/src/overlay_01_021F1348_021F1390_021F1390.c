#include "global.h"
#include "overlay_01_021F1348.h"

#include "constants/heap.h"

void ov01_021F1648(FieldEffectManager *fieldEffMan, enum HeapID heapID, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

void ov01_021F1390(FieldEffectManager *fieldEffMan, enum HeapID heapID, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

void ov01_021F1390(FieldEffectManager *fieldEffMan, enum HeapID heapID, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
    ov01_021F1648(fieldEffMan, heapID, a2, a3, a4, a5, a6, a7, a8);
}

void ov01_021F13B0(FieldEffectManager *fieldEffMan, u32 id) {
    if (ov01_021F1588(fieldEffMan, id) != NULL) {
        GF_AssertFail();
        return;
    }
    ov01_021F14F4(fieldEffMan, id);
}
