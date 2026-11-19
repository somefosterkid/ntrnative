#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

void ov112_021E7AC8(BgConfig *arg0);

void ov112_021E7AC8(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    GfGfx_EngineBTogglePlanes(0x1BU, 0U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    Heap_FreeExplicit(HEAP_ID_154, bgConfig);
}
