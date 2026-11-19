#include "global.h"

#include "error_handling.h"
#include "billboard_gfx_sequence.h"
#include "billboard.h"

fx32 sub_02024394(const Billboard *billboard, int animNum);
void sub_02020764(BillboardVRAMTransfer *transfer, const fx32 indexFx32);

fx32 sub_02023F70(const Billboard *billboard);

NNSG3dResMdl *sub_02023F90(const Billboard *billboard);

NNSGfdTexKey sub_02023FA0(const Billboard *billboard);
NNSGfdPlttKey sub_02023FB0(const Billboard *billboard);
void sub_02023FC0(Billboard *billboard);
NNSG3dResMdl *sub_02023FDC(Billboard *billboard);
void sub_02023FE4(Billboard *billboard, BillboardCallback callback, void *callbackParam);

fx32 sub_02023F70(const Billboard *billboard) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return billboard->frameNum - sub_02024394(billboard, billboard->animNum);
}

NNSG3dResMdl *sub_02023F90(const Billboard *billboard) {
    ((billboard) ? (void)0 : GF_AssertFail());
    return billboard->model;
}

NNSGfdTexKey sub_02023FA0(const Billboard *billboard) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return billboard->texKey;
}

NNSGfdPlttKey sub_02023FB0(const Billboard *billboard) {
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return billboard->plttKey;
}

void sub_02023FC0(Billboard *billboard) {
    if (billboard->state == BILLBOARD_STATE_VRAM_TRANSFER) {
        sub_02020764(billboard->vramTransfer, billboard->frameNum);
    }
}

NNSG3dResMdl *sub_02023FDC(Billboard *billboard) {
    return billboard->model;
}

void sub_02023FE4(Billboard *billboard, BillboardCallback callback, void *callbackParam) {
    billboard->callbackParam = callbackParam;
    billboard->callback = callback;
}
