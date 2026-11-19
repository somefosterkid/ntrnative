#include "global.h"

#include "overlay_98.h"

u8 ov98_0221F150(s32 arg0) {
    s32 v0;
    u8 v1;

    v0 = arg0;
    v1 = 0;
    if (v0 == 0) {
        return 1U;
    }
loop_3:
    if (v0 != 0) {
        v0 = v0 / 10;
        v1 += 1;
        goto loop_3;
    }
    return v1;
}
