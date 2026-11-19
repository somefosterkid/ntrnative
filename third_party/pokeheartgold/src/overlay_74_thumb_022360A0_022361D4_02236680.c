#include "global.h"

#include "overlay_74_wh_work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/overlay_74_thumb_022352A0_0223E2FC.s: 8 bytes on the DS, the work pointer
// and the print callback). The asm label stays for the DS build.
WHStatics ov74_0223E2FC;
#endif

BOOL ov74_02236680(void *userData, WHCallback callback, void *workBuf) {
    // 32-aligned inside the ov74_02236980() = sizeof + 0x20 byte block
    WHWork *v0 = (WHWork *)(((size_t)workBuf + 0x1F) & ~(size_t)0x1F);

    ov74_0223E2FC.work = v0;
    v0->state = 0;
    ov74_0223E2FC.work->phase = 1;
    ov74_0223E2FC.work->recvBuf = userData;
    ov74_0223E2FC.work->blockCount = 0;
    ov74_0223E2FC.work->blocksReceived = 0;
    ov74_0223E2FC.work->callback = callback;
    ov74_0223E2FC.work->channel = 0x10;
    ov74_0223E2FC.work->hasParentInfo = 0;
    ov74_0223E2FC.work->linkLevel = -1;
    return ov74_022364F0();
}

void ov74_022366E8(WHBlock *block) {
    s32 v0;

    ov74_0223E2FC.work->hasParentInfo = 0;
    ov74_0223E2FC.work->phase = 3;
    ov74_0223E2FC.work->checksum = block->checksum;
    ov74_0223E2FC.work->dataSize = block->size;
    ov74_0223E2FC.work->blockCount = (u16)(((u32)(ov74_0223E2FC.work->dataSize - 1) / 104U) + 1);
    v0 = 0;
    ov74_0223E2FC.work->blocksReceived = 0;
    ov74_0223E2FC.work->mismatchCount = 0;
    if ((s32)ov74_0223E2FC.work->blockCount > 0) {
        do {
            ov74_0223E2FC.work->blockReceived[v0] = 0;
            v0 += 1;
        } while (v0 < (s32)ov74_0223E2FC.work->blockCount);
    }
}
