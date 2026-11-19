#include "global.h"

#include "error_handling.h"
#include "overlay_01_022031C0.h"

void ov01_022039BC(u32 direction, VecFx32 *pos, fx32 **coordPtr) {
    switch (direction) {
    case 0:
    case 1:
        *coordPtr = &pos->z;
        return;
    case 2:
    case 3:
        *coordPtr = &pos->x;
    default:
        return;
    }
}

s32 ov01_022039E0(u32 arg0);

s32 ov01_022039E0(u32 arg0) {
    switch (arg0) {
    case 0:
        return -1;
    case 1:
        return 1;
    case 2:
        return -0xA;
    case 3:
        return 0xA;
    default:
        GF_AssertFail();
        return 0;
    }
}
