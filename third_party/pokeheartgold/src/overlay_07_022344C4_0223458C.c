#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_particle_effect.h"

s32 ov07_0221C69C(void);

UnkStruct_ov12_02237F98 *ov07_0223458C(enum HeapID heapID, UnkStruct_ov12_02237EFC *param1) {
    s32 v1;
    UnkStruct_ov12_02237F98 *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov12_02237F98));
    if (v0 == NULL) {
        GF_AssertFail();
        return NULL;
    }
    ov07_0221C69C();
    v0->heapID = heapID;
    if (param1 != NULL) {
        v0->unk_04 = *param1;
    }
    v1 = 0;
    do {
        v0->unk_08[v1] = NULL;
        v1 += 1;
    } while (v1 < 0x10);
    return v0;
}
