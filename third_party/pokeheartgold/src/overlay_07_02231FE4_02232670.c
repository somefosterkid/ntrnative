#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_ball_capsule_seal_effect.h"

BallCapsuleSealEffect *ov07_02232670(enum HeapID heapID);

BallCapsuleSealEffect *ov07_02232670(enum HeapID heapID) {
    BallCapsuleSealEffect *v0;

    v0 = Heap_Alloc(heapID, sizeof(BallCapsuleSealEffect));
    if (v0 == NULL) {
        GF_AssertFail();
        return NULL;
    }
    v0->heapID = heapID;
    v0->isActive = 0;
    return v0;
}
