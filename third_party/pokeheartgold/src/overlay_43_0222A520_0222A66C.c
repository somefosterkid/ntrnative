#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_43_palpad.h"

void ov43_0222A66C(PalPadGfx *gfx);

void ov43_0222A66C(PalPadGfx *gfx) {
    u32 bgId;

    bgId = 0;
    do {
        FreeBgTilemapBuffer(gfx->bgConfig, bgId);
        bgId += 1;
    } while ((s32)bgId < 7);
    Heap_Free(gfx->bgConfig);
    gfx->bgConfig = NULL;
}
