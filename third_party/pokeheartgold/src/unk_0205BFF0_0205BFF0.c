#include "global.h"

s32 sub_0205BFF0(u8 param_1, u8 param_2);

s32 sub_0205C01C(s32 arg0, u32 arg1, s32 arg2);

s32 sub_0205BFF0(u8 param_1, u8 param_2) {
    s32 in_r2;

    switch (param_2) {
    case 0:
        in_r2 = 0xb;
        break;
    case 1:
        in_r2 = 0x13;
        break;
    case 2:
        in_r2 = 0x1b;
        break;
    case 3:
        in_r2 = 0x73;
    }
    return in_r2 + (u32)param_1 * 4;
}

s32 sub_0205C01C(s32 arg0, u32 arg1, s32 arg2) {
    s32 v0;

    v0 = arg2;
    switch (arg1) {
    case 0:
        v0 = 0xA;
        break;
    case 1:
        v0 = 0x12;
        break;
    case 2:
        v0 = 0x1A;
        break;
    case 3:
        v0 = 0x72;
        break;
    }
    return v0 + (arg0 * 4);
}
