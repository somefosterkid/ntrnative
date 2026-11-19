#include "global.h"
#include "constants/heap.h"
#include "overlay_07_bg_scroll_context.h"

struct BufferManager;
BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);
void *sub_02014A4C(const BufferManager *bufferManager);
const void *sub_02014A60(const BufferManager *bufferManager);
void sub_02014A78(BufferManager *bufferManager, u8 mode);
void sub_02014A8C(BufferManager *bufferManager);
void sub_02014AA0(void);
void sub_02014AB0(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType);

void *sub_02014A4C(const BufferManager *bufferManager)
{
    ((bufferManager) ? (void)0 : GF_AssertFail()) ;
    return bufferManager->buffers[bufferManager->writeBuffer];
}

const void *sub_02014A60(const BufferManager *bufferManager)
{
    ((bufferManager) ? (void)0 : GF_AssertFail()) ;
    return bufferManager->buffers[bufferManager->writeBuffer ^ 1];
}

void sub_02014A78(BufferManager *bufferManager, u8 mode)
{
    ((bufferManager) ? (void)0 : GF_AssertFail()) ;
    bufferManager->mode = mode;
}

void sub_02014A8C(BufferManager *bufferManager)
{
    if (bufferManager == 0L ) {
        return;
    }

    if (bufferManager->mode == 0 ) {
        return;
    }

    bufferManager->writeBuffer ^= 1;
}

void sub_02014AA0(void)
{
    MI_StopDma(0);
    MI_WaitDma(0);
}

void sub_02014AB0(const void *src, void *dst, int size, enum BufferManagerDMAType dmaType)
{
    if (dmaType == BUFFER_MANAGER_DMA_TYPE_32BIT) {
        MI_HBlankDmaCopy32(0, src, dst, size);
    } else {
        MI_HBlankDmaCopy16(0, src, dst, size);
    }
}
