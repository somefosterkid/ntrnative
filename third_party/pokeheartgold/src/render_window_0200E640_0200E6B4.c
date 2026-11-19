#include "global.h"

#include "bg_window.h"
#include "pokemon_types_def.h"

struct BgConfig;

struct Pokemon;

u8 *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, u16 species, u8 gender, enum HeapID heapID);

u8 *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 palette, u16 baseTile, Pokemon *mon, enum HeapID heapID);

void sub_0200E6B4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile);

void sub_0200E6B4(BgConfig *bgConfig, u8 bgLayer, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 tile) {
    // clang-format off
    FillBgTilemapRect(bgConfig, bgLayer, tile,      x - 2,         y - 1,      1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 1,  x - 1,         y - 1,      1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 2,  x,             y - 1,      width, 1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 3,  x + width,     y - 1,      1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 4,  x + width + 1, y - 1,      1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 5,  x + width + 2, y - 1,      1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 6,  x - 2,         y,          1,     height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 7,  x - 1,         y,          1,     height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 9,  x + width,     y,          1,     height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 10, x + width + 1, y,          1,     height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 11, x + width + 2, y,          1,     height, palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 12, x - 2,         y + height, 1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 13, x - 1,         y + height, 1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 14, x,             y + height, width, 1,    palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 15, x + width,     y + height, 1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 16, x + width + 1, y + height, 1,     1,      palette);
    FillBgTilemapRect(bgConfig, bgLayer, tile + 17, x + width + 2, y + height, 1,     1,      palette);
    // clang-format on
}
