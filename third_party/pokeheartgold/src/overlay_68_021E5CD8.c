#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"

void ov68_021E5CD8(BgConfig *arg0);

void ov68_021E5CD8(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x1DU, 0U);
    FreeBgTilemapBuffer(bgConfig, 7U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    Heap_FreeExplicit(HEAP_ID_66, bgConfig);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
}
