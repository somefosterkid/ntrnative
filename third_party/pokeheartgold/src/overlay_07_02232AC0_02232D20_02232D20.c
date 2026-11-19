#include "global.h"

#include "assert.h"
#include "error_handling.h"
#include "heap.h"
#include "overlay_07.h"
#include "overlay_07_unk_struct_ov07_02232D20.h"
#include "palette.h"
#include "seal_case.h"
#include "sprite_system.h"
#include "unk_02014DA0.h"

extern int ov07_0223261C(int a0);

extern int ov07_02232630(int a0);

extern ParticleSystem *ov07_0221FEB0(enum HeapID heapId, int a1, int a2, int a3);

UnkStruct_ov07_02232D20 *ov07_02232D20(UnkStruct_ov07_02232D20_in *a0);

UnkStruct_ov07_02232D20 *ov07_02232D20(UnkStruct_ov07_02232D20_in *a0) {
    UnkStruct_ov07_02232D20 *v0 = Heap_Alloc(a0->unk8, sizeof(UnkStruct_ov07_02232D20));
    GF_ASSERT(v0 != NULL);

    v0->unk0 = *a0;

    if (v0->unk0.unk10) {
        v0->fileId = ov07_0223261C(0x403);
        v0->resourceCount = ov07_02232630(0x403);
    } else {
        v0->fileId = ov07_0223261C(v0->unk0.unk4);
        v0->resourceCount = ov07_02232630(v0->unk0.unk4);
    }

    v0->particleSystem = ov07_0221FEB0(v0->unk0.unk8, 0x5F, v0->fileId, 0);

    return v0;
}
