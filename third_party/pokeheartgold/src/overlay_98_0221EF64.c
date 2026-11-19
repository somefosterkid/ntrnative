#include "global.h"

#include "overlay_98.h"


extern u32 ov98_0221F220;

s32 ov98_0221EF64(u32 arg0) {
    if (arg0 >= 0x1C2U) {
        return 2;
    }
    if (arg0 >= 0x1A4U) {
        return 1;
    }
    return 0;
}

u32 ov98_0221EF80(s32 param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        if (*(s32 *)(&ov98_0221F220 + uVar1) <= param_1) {
            break;
        }
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 10);
    return 10 - uVar1 & 0xff;
}
