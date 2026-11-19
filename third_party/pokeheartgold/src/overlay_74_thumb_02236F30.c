#include "global.h"

u16 ov74_02236F30(u32 language);

s32 ov74_02236F44(u32 arg0);
s32 ov74_02236F80(u32 arg0);

u16 ov74_02236F30(u32 language) {
    u32 new_var;
    char new_var2;
    new_var = 1;
    new_var2 = new_var;
    if (language == 1) {
        language = new_var2;
    } else {
        language = 0x1DE;
    }
    return language;
}

s32 ov74_02236F44(u32 arg0) {
    switch (arg0) {
    default:
        return 0xEA;
    case 2:
    case 4:
    case 7:
        return 0x1B4;
    case 3:
        return 0x1B7;
    case 5:
        return 0x1B6;
    }
}

s32 ov74_02236F80(u32 arg0) {
    switch (arg0) {
    default:
        return 0xEB;
    case 2:
    case 4:
    case 7:
        return 0x1B5;
    case 3:
        return 0x1B8;
    case 5:
        return 0x1B4;
    }
}
