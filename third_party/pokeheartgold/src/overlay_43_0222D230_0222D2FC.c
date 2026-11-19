#include "global.h"

#include "system.h"

s32 ov43_0222D24C(s16 *, void *, s32, s32);

s32 ov43_0222ED18(void *, s32);

s32 ov43_0222D2FC(void *arg0, void *arg1, s32 arg2);

s32 ov43_0222D2FC(void *arg0, void *arg1, s32 arg2) {
    int new_var;
    u8 temp_r1;
    if (0xCF3 & gSystem.newKeys) {
        *((s32 *)arg1) = 0;
    }
    if (3 & gSystem.newKeys) {
        return ov43_0222D24C(arg0, arg1, arg2, 1);
    }
    new_var = gSystem.newAndRepeatedKeys;
    if ((new_var & 0x20) || ((temp_r1 = *((u8 *)(((u8 *)arg1) + 0xE)), temp_r1 != 0) && (0x200 & new_var))) {
        ov43_0222ED18(arg0, -1);
        return ov43_0222D24C(arg0, arg1, arg2, 2);
    }
    if ((new_var & 0x10) || ((temp_r1 != 0) && (new_var & 0x100))) {
        ov43_0222ED18(arg0, 1);
        return ov43_0222D24C(arg0, arg1, arg2, 3);
    }
    if (0x40 & gSystem.heldKeys) {
        return ov43_0222D24C(arg0, arg1, arg2, 4);
    }
    if (0x80 & gSystem.heldKeys) {
        return ov43_0222D24C(arg0, arg1, arg2, 5);
    }
    return 2;
}
