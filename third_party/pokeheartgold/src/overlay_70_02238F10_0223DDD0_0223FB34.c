#include "global.h"

#include "bg_window.h"

void ov70_0223FB34(BgConfig *arg0);

void ov70_0223FB34(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
}
