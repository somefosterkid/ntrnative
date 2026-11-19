#include "global.h"

#include "overlay_40.h"

void ov40_022334F8(u16 *param0, u16 param1, u16 param2) {
    int v0, v1, v2, v3;
    u32 v4;

    for (v0 = 0; v0 < param1; v0++) {
        param0++;
    }

    for (; v0 < param1 + param2; v0++) {
        v1 = ((*param0) & 0x1f);
        v2 = (((*param0) >> 5) & 0x1f);
        v3 = (((*param0) >> 10) & 0x1f);

        v4 = (((v1) * 76 + (v2) * 151 + (v3) * 29) >> 8);

        *param0 = (u16)((v4 << 10) | (v4 << 5) | v4);
        param0++;
    }
}
