#include "global.h"
#include "heap.h"
#include "constants/heap.h"
#include "overlay_07_bg_scroll_context.h"

BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);

BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2) {
    BufferManager *bufferManager = Heap_Alloc(heapID, sizeof(BufferManager));
#if defined(SDK_PORT) || defined(NONMATCHING)
    // LP64: buffers[] are pointers, sizeof(BufferManager) is 0x18, not 0xC
    MI_CpuClear8(bufferManager, sizeof(BufferManager));
#else
    u8 *p = (u8 *)bufferManager;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    p[8] = 0;
    p[9] = 0;
    p[10] = 0;
    p[11] = 0;
#endif
    bufferManager->mode = 1;
    bufferManager->buffers[0] = buffer1;
    bufferManager->buffers[1] = buffer2;
    return bufferManager;
}
