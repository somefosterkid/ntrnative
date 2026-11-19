#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"

static void sub_0200E448(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);

static void sub_0200E448(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);
void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette);
void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);
u32 sub_0200E63C(u32 messageBoxFrame);

u32 sub_0200E640(s32 frame);

static void sub_0200E448(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {

    FillBgTilemapRect(bgConfig, bgLayer, tile, x - 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 1, x, y - 1, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 2, x + width, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 3, x - 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 5, x + width, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 6, x - 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 7, x, y + height, width, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 8, x + width, y + height, 1, 1, palette);
}

void DrawFrameAndWindow1(Window *window, u8 skipTransfer, u16 tile, u8 palette) {
    sub_0200E448(window->bgConfig,
        GetWindowBgId(window),
        GetWindowX(window),
        GetWindowY(window),
        GetWindowWidth(window),
        GetWindowHeight(window),
        palette,
        tile);

    if (skipTransfer == 0) {
        CopyWindowToVram(window);
    }
}

void ClearWindowPixelBuffer(Window *window, u8 skipTransfer) {
    FillBgTilemapRect(window->bgConfig,
        GetWindowBgId(window),
        0,
        GetWindowX(window) - 1,
        GetWindowY(window) - 1,
        GetWindowWidth(window) + 2,
        GetWindowHeight(window) + 2,
        0);

    if (skipTransfer == 0) {
        ClearWindowTilemapAndCopyToVram(window);
    }
}

u32 sub_0200E63C(u32 messageBoxFrame) {
    return 2 + messageBoxFrame;
}

u32 sub_0200E640(s32 frame) {
    return frame + 0x1A;
}
