#include "global.h"

#include "overlay_80_02236450.h"

s32 ov80_02236B30();

s32 ov80_02236AF0(void) {
    if (ov80_02236B30() == 0) {
        return 0x81;
    }
    return 0xCD;
}

s32 ov80_02236B04(void);

s32 ov80_02236B04(void) {
    if (ov80_02236B30() == 0) {
        return 0x80;
    }
    return 0xCC;
}

s32 ov80_02236B18(void) {
    if (ov80_02236B30() == 0) {
        return 0x2D3;
    }
    return 0x2D4;
}
