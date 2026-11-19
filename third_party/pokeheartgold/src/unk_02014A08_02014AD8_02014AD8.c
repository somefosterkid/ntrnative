#include "global.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);

ScreenScrollManager *sub_02014AD8(enum HeapID heapID);

ScreenScrollManager *sub_02014AD8(enum HeapID heapID) {
    ScreenScrollManager *screenScrollMgr;

    screenScrollMgr = Heap_Alloc(heapID, sizeof(ScreenScrollManager));
    memset(screenScrollMgr, 0, sizeof(ScreenScrollManager));
    screenScrollMgr->bufferManager = sub_02014A08(heapID, screenScrollMgr->buffer1, screenScrollMgr->buffer2);
    return screenScrollMgr;
}
