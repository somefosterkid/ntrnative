#include "global.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

void sub_02014A38(BufferManager *bufferManager);

void sub_02014B9C(ScreenScrollManager *screenScrollMgr);

void sub_02014D68(void);

void sub_02014BD8(ScreenScrollManager *screenScrollMgr);

void sub_02014BD8(ScreenScrollManager *screenScrollMgr) {
    sub_02014B9C(screenScrollMgr);
    sub_02014D68();
    sub_02014A38(screenScrollMgr->bufferManager);
    Heap_Free(screenScrollMgr);
}
