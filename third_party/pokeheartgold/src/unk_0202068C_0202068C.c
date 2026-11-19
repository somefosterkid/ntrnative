#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "unk_0202068C.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void sub_02020770(BillboardVRAMTransfer *transfer);

void sub_02020740(BillboardListVRAMTransfer *listTransfer);

void sub_02020780(BillboardVRAMTransfer *transfer, const u16 index);
BillboardVRAMTransfer *sub_020206E0(BillboardListVRAMTransfer *listTransfer, const BillboardGfxSequence *gfxSequence, const NNSG3dResTex *texture, NNSGfdTexKey texKey, NNSGfdPlttKey plttKey, const fx32 index);

BillboardListVRAMTransfer *sub_0202068C(int maxElements, enum HeapID heapID) {
    BillboardListVRAMTransfer *listTransfer = Heap_Alloc(heapID, sizeof(BillboardListVRAMTransfer));

    listTransfer->billboardTransfers = Heap_Alloc(heapID, maxElements * sizeof(BillboardVRAMTransfer));
    listTransfer->maxElements = maxElements;
    for (int i = 0; i < listTransfer->maxElements; i++) {
        sub_02020770(listTransfer->billboardTransfers + i);
    }
    return listTransfer;
}

void sub_020206C8(BillboardListVRAMTransfer *listTransfer) {
    sub_02020740(listTransfer);
    Heap_Free(listTransfer->billboardTransfers);
    Heap_Free(listTransfer);
}

BillboardVRAMTransfer *sub_020206E0(BillboardListVRAMTransfer *listTransfer, const BillboardGfxSequence *gfxSequence, const NNSG3dResTex *texture, NNSGfdTexKey texKey, NNSGfdPlttKey plttKey, const fx32 index) {
    BillboardVRAMTransfer *transfer = 0L;

    for (int i = 0; i < listTransfer->maxElements; i++) {
        if (listTransfer->billboardTransfers[i].gfxSequence == 0L) {
            transfer = &listTransfer->billboardTransfers[i];
            break;
        }
    }

    if (transfer == 0L) {
        return 0L;
    }

    transfer->gfxSequence = gfxSequence;
    transfer->texture = texture;
    transfer->texKey = texKey;
    transfer->plttKey = plttKey;
    transfer->textureIdx = -1;
    transfer->plttIdx = -1;

    sub_02020780(transfer, index >> 12);

    return transfer;
}
