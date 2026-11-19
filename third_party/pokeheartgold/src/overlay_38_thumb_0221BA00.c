#include "global.h"

#include "overlay_38_thumb.h"

extern s32 _02226020;

void ov38_0221BA00(s32 arg0) {
    _02226020 = (arg0 << 0x10) | arg0;
}

u8 ov38_0221BA10(void) {
    s32 v0;

    v0 = 0x7FFFFFFF & ((_02226020 * 0x45) + 0x1111);
    _02226020 = v0;
    return (u8)(v0 >> 0x10);
}
