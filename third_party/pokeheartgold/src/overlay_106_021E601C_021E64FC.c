#include "global.h"

#include "bg_window.h"
#include "heap.h"

void ov106_021E64FC(BgConfig **arg0);

void ov106_021E64FC(BgConfig **arg0) {
    FreeBgTilemapBuffer(*arg0, 2U);
    FreeBgTilemapBuffer(*arg0, 1U);
    FreeBgTilemapBuffer(*arg0, 0U);
    Heap_Free(*arg0);
}
