#include "global.h"

#include "constants/heap.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "vram_transfer_manager.h"

void sub_02020770(BillboardVRAMTransfer *transfer);
void sub_02020780(BillboardVRAMTransfer *transfer, const u16 index);
static void sub_020207C8(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes);
static void sub_020207F4(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes);
void *sub_02020820(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndex);
void *sub_0202082C(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndex);

void sub_02020740(BillboardListVRAMTransfer *listTransfer);
void sub_02020764(BillboardVRAMTransfer *transfer, const fx32 indexFx32);
void sub_02020770(BillboardVRAMTransfer *transfer);
void sub_02020780(BillboardVRAMTransfer *transfer, const u16 index);
static void sub_020207C8(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes);
static void sub_020207F4(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes);

void sub_02020740(BillboardListVRAMTransfer *listTransfer) {
    for (int i = 0; i < listTransfer->maxElements; i++) {
        sub_02020770(listTransfer->billboardTransfers + i);
    }
}

void sub_02020764(BillboardVRAMTransfer *transfer, const fx32 indexFx32) {
    u16 index = indexFx32 >> 12;
    sub_02020780(transfer, index);
}

void sub_02020770(BillboardVRAMTransfer *transfer) {
    transfer->gfxSequence = 0L;
    transfer->texture = 0L;
    transfer->texKey = 0;
    transfer->plttKey = 0;
    transfer->textureIdx = 0;
    transfer->plttIdx = 0;
}

void sub_02020780(BillboardVRAMTransfer *transfer, const u16 index) {
    BillboardTexPlttIndex texPlttIndexes = BillboardGfxSequence_GetTexPlttIndexAt(transfer->gfxSequence, index);

    if (transfer->textureIdx != texPlttIndexes.textureIdx) {
        sub_020207C8(transfer, &texPlttIndexes);
    }

    if (transfer->plttIdx != texPlttIndexes.plttIdx) {
        sub_020207F4(transfer, &texPlttIndexes);
    }
}

static void sub_020207C8(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes) {
    void *buffer = sub_02020820(transfer, texPlttIndexes);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, NNS_GfdGetTexKeyAddr(transfer->texKey), buffer, NNS_GfdGetTexKeySize(transfer->texKey));
    transfer->textureIdx = texPlttIndexes->textureIdx;
}

static void sub_020207F4(BillboardVRAMTransfer *transfer, const BillboardTexPlttIndex *texPlttIndexes) {
    void *buffer = sub_0202082C(transfer, texPlttIndexes);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, NNS_GfdGetPlttKeyAddr(transfer->plttKey), buffer, NNS_GfdGetPlttKeySize(transfer->plttKey));
    transfer->plttIdx = texPlttIndexes->plttIdx;
}
