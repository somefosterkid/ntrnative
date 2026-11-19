#include "global.h"
struct ScreenScrollManager;
#include "error_handling.h"
#include "overlay_07_bg_scroll_context.h"

const void *sub_02014A60(const BufferManager *bufferManager);

void sub_02014AB0(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType);

void sub_02014D70(struct ScreenScrollManager *arg0);

void sub_02014D70(ScreenScrollManager *arg0) {
    const void *temp_r0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    temp_r0 = sub_02014A60(arg0->bufferManager);
    DC_FlushRange(temp_r0, 0x300U);
    sub_02014AB0(temp_r0, arg0->offsetRegisterAddr, 4, BUFFER_MANAGER_DMA_TYPE_32BIT);
}
