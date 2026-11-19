#include "global.h"
struct HallOfFameShowcase;

#include "bg_window.h"
#include "heap.h"
#include "overlay_64_hall_of_fame_showcase.h"

void ov64_021E5CA4(struct HallOfFameShowcase *arg0);

void ov64_021E5CA4(HallOfFameShowcase *arg0) {
    FreeBgTilemapBuffer(arg0->bgConfig, 5U);
    FreeBgTilemapBuffer(arg0->bgConfig, 4U);
    FreeBgTilemapBuffer(arg0->bgConfig, 1U);
    FreeBgTilemapBuffer(arg0->bgConfig, 0U);
    Heap_Free(arg0->bgConfig);
}
