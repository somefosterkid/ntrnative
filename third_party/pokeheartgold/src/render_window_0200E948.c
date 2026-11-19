#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"

void sub_0200E6B4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);

void sub_0200E948(Window *window, u32 tile, u32 palette);

void sub_0200E948(Window *window, u32 tile, u32 palette) {
    sub_0200E6B4(window->bgConfig,
        GetWindowBgId(window),
        GetWindowX(window),
        GetWindowY(window),
        GetWindowWidth(window),
        GetWindowHeight(window),
        palette,
        tile);
}
