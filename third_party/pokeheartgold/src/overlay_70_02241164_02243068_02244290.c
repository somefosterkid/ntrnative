#include "global.h"

#include "bg_window.h"

void ov70_02244290(BgConfig *arg0);

void ov70_02244290(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
}
