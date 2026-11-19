#include "global.h"

#include "error_handling.h"

s32 ov02_0224F79C(u32 arg0);

s32 ov02_0224F79C(u32 arg0) {
    switch (arg0) {
    case 0:
        return 1;
    case 1:
        return 7;
    case 2:
        return 0xA;
    case 3:
        return 8;
    case 4:
        return 9;
    case 5:
        return 0xD;
    case 6:
        return 0xC;
    case 7:
        return 0xE;
    case 8:
        return 0x11;
    case 10:
        return 2;
    case 11:
        return 3;
    case 12:
        return 5;
    case 13:
        return 4;
    case 14:
        return 0xB;
    case 15:
        return 6;
    case 16:
        return 0xF;
    case 17:
        return 0x10;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov02_0224F820(u32 arg0);

s32 ov02_0224F820(u32 arg0) {
    switch (arg0) {
    case 0:
        return 4;
    case 1:
        return 2;
    case 2:
        return 1;
    case 3:
        return 7;
    case 4:
        return 6;
    case 5:
        return 5;
    case 6:
        return 3;
    default:
        GF_AssertFail();
        return 8;
    }
}
