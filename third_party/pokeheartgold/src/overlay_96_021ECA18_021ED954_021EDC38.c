#include "global.h"

u32 ov96_021EDC38(u32 param_1, s32 param_2);

u32 ov96_021EDC38(u32 param_1, s32 param_2) {
    u32 uVar1;
    switch (param_1) {
    case 10:
        return 0;

    case 5:
        if (param_2 != 0) {
            uVar1 = 4;
        } else {
            uVar1 = 0;
        }
        return (u8)uVar1;

    case 6:
        if (param_2 != 0) {
            uVar1 = 2;
        } else {
            uVar1 = 4;
        }
        return (u8)uVar1;

    case 7:
        if (param_2 != 0) {
            uVar1 = 1;
        } else {
            uVar1 = 2;
        }
        return (u8)uVar1;

    case 8:
        if (param_2 != 0) {
            uVar1 = 3;
        } else {
            uVar1 = 1;
        }
        return (u8)uVar1;

    case 9:
        if (param_2 != 0) {
            uVar1 = 0;
        } else {
            uVar1 = 3;
        }
        return (u8)uVar1;

    default:
        return param_1;
    }
}
