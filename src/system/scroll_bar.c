// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/scroll_bar.c, which is the same code in
// Black 2/White 2's main module. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "system/scroll_bar.h"
#include "types.h"

// A scroll bar's conversions between a value and the bar's position, with 8 bits of fraction. The file's name is
// pokebw2's guess

// ScrollBar_GetValue
u32 func_0202ede4(u32 max, u32 pos, u32 top, u32 bottom, u32 barSize) {
    u32 start = top + barSize / 2;
    u32 end = bottom - barSize / 2;

    if (pos <= start) {
        return 0;
    }
    if (pos >= end) {
        return max;
    }
    pos -= start;
    max <<= 8;
    return (pos * max / (end - start)) >> 8;
}

// ScrollBar_GetPos
u32 func_0202ee0c(u32 max, u32 value, u32 top, u32 bottom, u32 barSize) {
    u32 range = (bottom - top - barSize) << 8;

    value = (range * value / max) >> 8;
    return top + value + barSize / 2;
}
