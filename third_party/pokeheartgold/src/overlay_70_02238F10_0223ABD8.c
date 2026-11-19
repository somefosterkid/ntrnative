#include "global.h"

#include "bg_window.h"

s32 ov70_022392BC(BgConfig *bgConfig);

void ov70_0223ABD8(BgConfig *arg0);

void ov70_0223ABD8(BgConfig *bgConfig) {
    ov70_022392BC(bgConfig);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
}
