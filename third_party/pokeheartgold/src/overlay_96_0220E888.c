#include "global.h"

s32 ov96_0220E888(s32 param_1);

void ov96_0220E890(s32 *arg0, s32 arg1);

s32 ov96_0220E888(s32 param_1) {
    return param_1 * param_1;
}

void ov96_0220E890(s32 *arg0, s32 arg1) {
    s32 v0;
    s32 v1;

    v0 = *arg0;
    if (v0 != 0) {
        if (v0 < 0) {
            v1 = 0 - v0;
        } else {
            v1 = v0;
        }
        if (v1 < arg1) {
            *arg0 = 0;
            return;
        }
        *arg0 -= (v0 > 0 ? 1 : -1) * arg1;
    }
}
