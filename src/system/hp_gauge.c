// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/hp_gauge.c, which is the same code in
// Black 2/White 2's main module. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "system/hp_gauge.h"
#include "types.h"

// The HP gauges' size and color. The ROM doesn't name the file; hp_gauge.c is pokebw2's guess

// HPGauge_GetFill
u8 func_0202d01c(u32 hp, u32 maxHp, u32 width) {
    u8 pixels = hp * width / maxHp;

    if (pixels == 0 && hp != 0) {
        pixels = 1;
    }
    return pixels;
}

// HPGauge_GetColor
u8 func_0202d034(u32 hp, u32 maxHp) {
    // In 1/256ths, so that the fractions keep their precision
    hp <<= 8;
    maxHp <<= 8;
    if (hp > maxHp / 2) {
        return HP_GAUGE_COLOR_GREEN;
    }
    if (hp > maxHp / 5) {
        return HP_GAUGE_COLOR_YELLOW;
    }
    if (hp != 0) {
        return HP_GAUGE_COLOR_RED;
    }
    return HP_GAUGE_COLOR_NONE;
}
