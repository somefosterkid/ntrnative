#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "unk_020932E0.h"

UnkStruct_020932E0 *sub_020932E0(enum HeapID heapID, u8 arg1, s8 arg2) {
    UnkStruct_020932E0 *v0;

    v0 = Heap_Alloc(heapID, sizeof(UnkStruct_020932E0));
    memset(v0, 0, sizeof(UnkStruct_020932E0));
    if (arg1 == 0) {
        GF_AssertFail();
    }
    v0->unk_4 = heapID;
    v0->unk_D = arg1;
    v0->unk_10 = arg2;
    if ((u32) v0->unk_D > 3U) {
        GF_AssertFail();
        v0->unk_D = 3U;
    }
    v0->unk_8D0 = Heap_Alloc(v0->unk_4, sizeof(UnkStruct_020932E0_SlotC) * v0->unk_D);
    v0->unk_7E4 = Heap_Alloc(v0->unk_4, (v0->unk_D + 0x22) * sizeof(TouchscreenHitbox));
    v0->unk_46B8 = sub_02095CE0(v0->unk_4, &v0->unk_F);
    return v0;
}
