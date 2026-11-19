#include "global.h"

#include "bg_window.h"

void ov33_0225D7B8(BgConfig *arg0);

void ov33_0225D7B8(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
}
