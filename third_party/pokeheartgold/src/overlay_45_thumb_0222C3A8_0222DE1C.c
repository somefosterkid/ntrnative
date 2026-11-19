#include "global.h"

s32 ov45_0222DE3C(void *, s32, s32, s32);

void ov45_0222DE1C(void *arg0);

void ov45_0222DE1C(void *arg0) {
    s32 v0;

    v0 = 0;
    do {
        ov45_0222DE3C(arg0, v0, 2, 0);
        v0 += 1;
    } while (v0 < 0x14);
}
