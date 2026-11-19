#include "global.h"

#include "math_util.h"

s16 ov96_0220E6DC(int angle, int param_2);

s16 ov96_0220E6DC(int angle, int param_2) {
    s16 base;
    base = (angle * (param_2 - 7) * 38) / 1000 + 128;
    base += (GF_SinDeg(angle * 2) << 6) >> 16;
    return base;
}
