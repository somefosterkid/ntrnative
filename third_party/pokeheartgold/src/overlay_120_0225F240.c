#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_120.h"

u16 ov120_0225F240(Ov120Shuffle *shuffle) {
    u16 uVar1;
    s32 iVar2;
    u32 uVar3;

    iVar2 = shuffle->pos;
    shuffle->pos = iVar2 + 1;
    uVar1 = shuffle->order[iVar2];
    uVar3 = shuffle->count;
    if (shuffle->pos <= shuffle->count) {
        uVar3 = shuffle->pos;
    }
    shuffle->pos = uVar3;
    return uVar1;
}

Ov120Tiles *ov120_0225F268(enum HeapID heapID) {
    s32 v1;
    Ov120Tiles *v0;

    v0 = Heap_Alloc(heapID, sizeof(Ov120Tiles));
    memset(v0, 0, sizeof(Ov120Tiles));
    v1 = 0;
    do {
        v0->tiles[v1] = ov120_0225F020(heapID);
        v1 += 1;
    } while (v1 < 0x30);
    return v0;
}

void ov120_0225F294(Ov120Tiles *tiles) {
    s32 v0;

    if (tiles == NULL) {
        GF_AssertFail();
    }
    v0 = 0;
    do {
        ov120_0225F038(tiles->tiles[v0]);
        v0 += 1;
    } while (v0 < 0x30);
    Heap_Free(tiles);
}
