#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "unk_02088288.h"

void sub_0208877C(BgConfig *bgConfig) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    Heap_FreeExplicit(HEAP_ID_19, bgConfig);
}
