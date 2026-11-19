#include "global.h"

#include "bg_window.h"

void ov70_022392BC(BgConfig *arg0);

void ov70_022392BC(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 7U);
}
