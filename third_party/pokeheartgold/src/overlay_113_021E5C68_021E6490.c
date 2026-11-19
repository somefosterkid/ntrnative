#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_113.h"

void ov113_021E6490(UnownReportData *arg0);

void ov113_021E6490(UnownReportData *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 3U);
    FreeBgTilemapBuffer(arg0->bgConfig, 2U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
    FreeBgTilemapBuffer(arg0->bgConfig, 7U);
    FreeBgTilemapBuffer(arg0->bgConfig, 6U);
    FreeBgTilemapBuffer(arg0->bgConfig, 5U);
    Heap_Free(arg0->bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}
