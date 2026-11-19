#include "global.h"

void ov07_02222F7C(u16 *colors, u16 colorCount);

void ov07_02222F7C(u16 *colors, u16 colorCount) {
    for (int i = 0; i < colorCount; i++) {
        int r = (((*colors) >> (0)) & 31);
        int g = (((*colors) >> (5)) & 31);
        int b = (((*colors) >> (10)) & 31);
        u32 y = (((r) * 76 + (g) * 151 + (b) * 29) >> 8);
        *colors = (u16)((y << 10) | (y << 5) | y);
        colors++;
    }
}
