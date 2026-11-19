#include "global.h"
#include "filesystem_files_def.h"
#include "bg_window.h"

struct BgConfig;
enum SignpostType {
    SIGNPOST_TYPE_MAP = 0,
    SIGNPOST_TYPE_ARROW = 1,
    SIGNPOST_TYPE_LANDMARK = 2,
    SIGNPOST_TYPE_SCROLLING = 3,
};
u32 GfGfxLoader_LoadCharData(enum NarcId narcID, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);
void FillBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, u16 fillVal, u8 x, u8 y, u8 width, u8 height, u8 palette);
u8 GetWindowBgId(Window *window);
u8 GetWindowX(Window *window);
u8 GetWindowY(Window *window);

void sub_0200EC84(BgConfig *bgConfig, u8 bgLayer, u16 offset, u8 signpostType, u16 narcMemberIdx, u32 heapID);
void sub_0200ECBC(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);
void sub_0200EF84(Window *window, u16 tile, u8 palette);

void sub_0200EC84(BgConfig *bgConfig, u8 bgLayer, u16 offset, u8 signpostType, u16 narcMemberIdx, u32 heapID) {
    if (signpostType == SIGNPOST_TYPE_MAP) {
        narcMemberIdx += 33;
    } else {
        narcMemberIdx += 2;
    }

    GfGfxLoader_LoadCharData(NARC_a_0_3_6,
        narcMemberIdx,
        bgConfig,
        bgLayer,
        offset,
        24 * (8 * (8 / 2)),
        0,
        heapID);
}

void sub_0200ECBC(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {

    FillBgTilemapRect(bgConfig, bgLayer, tile, x - 9, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 1, x - 8, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 2, x - 7, y - 1, width + 7, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 3, x + width, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 4, x + width + 1, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 5, x + width + 2, y - 1, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 6, x - 9, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 7, x - 8, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 8, x - 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 9, x + width, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 10, x + width + 1, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 11, x + width + 2, y, 1, height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 12, x - 9, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 13, x - 8, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 14, x - 7, y + height, width + 7, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 15, x + width, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 16, x + width + 1, y + height, 1, 1, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 17, x + width + 2, y + height, 1, 1, palette);
}

void sub_0200EF84(Window *window, u16 tile, u8 palette) {
    u16 dy, dx;

    u8 bgLayer = GetWindowBgId(window);
    u16 x = GetWindowX(window) - 7;
    u16 y = GetWindowY(window);

    for (dy = 0; dy < 4; dy++) {
        for (dx = 0; dx < 6; dx++) {
            FillBgTilemapRect(window->bgConfig, bgLayer, tile + dy * 6 + dx, x + dx, y + dy, 1, 1, palette);
        }
    }
}
