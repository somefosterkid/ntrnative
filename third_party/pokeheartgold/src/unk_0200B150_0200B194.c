#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "oam_manager.h"


void sub_0200B194(int mainOamStart, int mainOamCount, int mainAffineOamStart, int mainAffineOamCount, int subOamStart, int subOamCount, int subAffineOamStart, int subAffineOamCount, enum HeapID heapID);

void sub_0200B194(int mainOamStart, int mainOamCount, int mainAffineOamStart, int mainAffineOamCount, int subOamStart, int subOamCount, int subAffineOamStart, int subAffineOamCount, enum HeapID heapID) {
    GF_ASSERT(_021D0EB0 == NULL);
    _021D0EB0 = Heap_Alloc(heapID, sizeof(OamManager));
    GF_ASSERT(_021D0EB0);

    _021D0EB0->heapID = heapID;
    GF_ASSERT(NNS_G2dGetNewOamManagerInstance(&_021D0EB0->mainScreenOam, mainOamStart, mainOamCount, mainAffineOamStart, mainAffineOamCount, NNS_G2D_OAMTYPE_MAIN));
    GF_ASSERT(NNS_G2dGetNewOamManagerInstance(&_021D0EB0->subScreenOam, subOamStart, subOamCount, subAffineOamStart, subAffineOamCount, NNS_G2D_OAMTYPE_SUB));
}
