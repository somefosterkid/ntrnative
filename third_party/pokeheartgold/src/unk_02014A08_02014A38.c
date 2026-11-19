#include "global.h"
#include "error_handling.h"
#include "heap.h"
#include "overlay_07_bg_scroll_context.h"

void sub_02014A38(BufferManager *bufferManager);

void sub_02014A38(BufferManager *bufferManager) {
    if (bufferManager == NULL) {
        GF_AssertFail();
    }
    Heap_Free(bufferManager);
}
