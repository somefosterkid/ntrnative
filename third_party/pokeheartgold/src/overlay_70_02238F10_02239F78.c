#include "global.h"

#include "bg_window.h"

s32 ov70_022392BC(BgConfig *bgConfig);

void ov70_02239F78(BgConfig *arg0);

void ov70_02239F78(BgConfig *bgConfig) {
    ov70_022392BC(bgConfig);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    FreeBgTilemapBuffer(bgConfig, 3U);
}
