#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"

void ov08_0221CF08(BgConfig *arg0);

void ov08_0221CF08(BgConfig *bgConfig) {
    GfGfx_EngineBTogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 7U);
}
