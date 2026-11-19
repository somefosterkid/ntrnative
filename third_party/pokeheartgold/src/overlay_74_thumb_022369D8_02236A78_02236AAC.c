#include "global.h"

extern u32 ov74_0223E304;

void ov74_02236AAC(u32 param_1, u32 param_2);

void ov74_02236AAC(u32 param_1, u32 param_2) {
    ov74_0223E304 = 1;
    ((u32 *)&ov74_0223E304)[3] = param_2;
    return;
}
