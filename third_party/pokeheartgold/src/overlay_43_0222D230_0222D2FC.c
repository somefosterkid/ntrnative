#include "global.h"

#include "system.h"
#include "overlay_43_palpad_fns.h"

s32 ov43_0222D2FC(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    int new_var;
    u8 temp_r1;
    if (0xCF3 & gSystem.newKeys) {
        shared->inputState = 0;
    }
    if (3 & gSystem.newKeys) {
        return ov43_0222D24C(details, shared, gfx, 1);
    }
    new_var = gSystem.newAndRepeatedKeys;
    if ((new_var & 0x20) || ((temp_r1 = shared->lrPaging, temp_r1 != 0) && (0x200 & new_var))) {
        ov43_0222ED18(details, -1);
        return ov43_0222D24C(details, shared, gfx, 2);
    }
    if ((new_var & 0x10) || ((temp_r1 != 0) && (new_var & 0x100))) {
        ov43_0222ED18(details, 1);
        return ov43_0222D24C(details, shared, gfx, 3);
    }
    if (0x40 & gSystem.heldKeys) {
        return ov43_0222D24C(details, shared, gfx, 4);
    }
    if (0x80 & gSystem.heldKeys) {
        return ov43_0222D24C(details, shared, gfx, 5);
    }
    return 2;
}
