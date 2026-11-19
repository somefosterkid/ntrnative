#include "global.h"

#include "pokemon_summary_app.h"

#include "bg_window.h"
#include "unk_0208805C.h"
#include "gf_gfx_planes.h"

enum BarColor {
    BARCOLOR_EMPTY = 0,
    BARCOLOR_RED,
    BARCOLOR_YELLOW,
    BARCOLOR_GREEN,

    BARCOLOR_MAX,
};

void sub_0208A0EC(PokemonSummary *summaryScreen);

void sub_0208A0EC(PokemonSummary *summaryScreen) {
    u16 baseTile;

    switch (CalculateHpBarColor(summaryScreen->mon.hp, summaryScreen->mon.maxHp, 48)) {
    case BARCOLOR_MAX:
    case BARCOLOR_GREEN:
    case BARCOLOR_EMPTY:
        baseTile = 0xF097;
        break;
    case BARCOLOR_YELLOW:
        baseTile = 0xF0B7;
        break;
    case BARCOLOR_RED:
        baseTile = 0xF0D7;
        break;
    }

    u8 pixelCount = CalculateHpBarPixelsLength(summaryScreen->mon.hp, summaryScreen->mon.maxHp, 48);
    u16 tile;

    for (u8 i = 0; i < 6; i++) {
        if (pixelCount >= 8) {
            tile = baseTile + 8;
        } else {
            tile = baseTile + pixelCount;
        }

        FillBgTilemapRect(summaryScreen->bgConfig, GF_BG_LYR_MAIN_3, tile, 10 + i, 5, 1, 1, 17);

        if (pixelCount < 8) {
            pixelCount = 0;
        } else {
            pixelCount -= 8;
        }
    }

    ScheduleBgTilemapBufferTransfer(summaryScreen->bgConfig, GF_BG_LYR_MAIN_3);
}

void sub_0208A1A0(PokemonSummary *summaryScreen);

void sub_0208A1A0(PokemonSummary *summaryScreen) {
    u32 maxExp;
    u32 curExp;

    if (summaryScreen->mon.level < 100) {
        maxExp = summaryScreen->mon.nextLevelExp - summaryScreen->mon.curLevelExp;
        curExp = summaryScreen->mon.exp - summaryScreen->mon.curLevelExp;
    } else {
        maxExp = 0;
        curExp = 0;
    }

    u8 pixelCount = CalculateHpBarPixelsLength(curExp, maxExp, 56);
    u16 tile;

    for (u8 i = 0; i < 7; i++) {
        if (pixelCount >= 8) {
            tile = 0xE03F;
        } else {
            tile = 0xE037 + pixelCount;
        }

        FillBgTilemapRect(summaryScreen->bgConfig, (enum GFBgLayer)6, tile, 9 + i, 19, 1, 1, 17);

        if (pixelCount < 8) {
            pixelCount = 0;
        } else {
            pixelCount -= 8;
        }
    }

    ScheduleBgTilemapBufferTransfer(summaryScreen->bgConfig, (enum GFBgLayer)6);
}
