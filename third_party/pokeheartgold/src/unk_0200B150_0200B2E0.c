#include "global.h"

#include "sys_task.h"
#include "error_handling.h"
#include "constants/heap.h"
#include "oam_manager.h"

void ClearMainOAM(enum HeapID heapID);
void ClearSubOAM(enum HeapID heapID);

void thunk_ClearMainOAM(enum HeapID heapID);
void thunk_ClearSubOAM(enum HeapID heapID);

BOOL sub_0200B2F0(const GXOamAttr *pOam, u16 affineIdx, BOOL doubleAffine);

void thunk_ClearMainOAM(enum HeapID heapID) {
    ClearMainOAM(heapID);
}

void thunk_ClearSubOAM(enum HeapID heapID) {
    ClearSubOAM(heapID);
}

BOOL sub_0200B2F0(const GXOamAttr *pOam, u16 affineIdx, BOOL doubleAffine) {
    BOOL v0;

    v0 = NNS_G2dEntryOamManagerOamWithAffineIdx(&_021D0EB0->mainScreenOam, pOam, affineIdx);
    if (v0 == 0) {
        GF_AssertFail();
    }
    return v0;
}
