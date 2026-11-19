#include "global.h"

extern s32 ov96_0221B058;

s32 ov96_021EDF5C(s32 arg0);

s32 ov96_021EDF7C(s32 arg0);

s32 ov96_021EDF5C(s32 arg0) {
    s32 v0;
    s32 *v1;

    v1 = &ov96_0221B058;
    v0 = 0;
loop_1:
    if ((s32)*v1 > arg0) {
        return v0;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 0xC) {
        return v0;
    }
    goto loop_1;
}

s32 ov96_021EDF7C(s32 arg0) {
    return (arg0 * 4) + 0x20;
}
