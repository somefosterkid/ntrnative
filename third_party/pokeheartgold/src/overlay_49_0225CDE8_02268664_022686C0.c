#include "global.h"

#include "overlay_49.h"

s32 ov49_02268640(s32, s32);

void ov49_022686C0(s32 arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    ov49_02268640(arg0, arg1);
    v2 = 0;
    v0 = arg1 + 2;
    v1 = arg0 + 8;
    do {
        ov49_02268640(v1, v0);
        v0 += 2;
        v1 += 4;
        v2 += 1;
    } while (v2 < 4);
}
