#include "global.h"

u32 sub_02032504(s32 param_1, u32 param_2);

u32 sub_02032504(s32 param_1, u32 param_2) {
    if (param_2 == 0) {
        return 0;
    }
    switch (param_1) {
    case 0:
        if (0x3e < param_2) {
            return 6;
        }
        if (0x32 < param_2) {
            return 5;
        }
        if (0x28 < param_2) {
            return 4;
        }
        if (0x1e < param_2) {
            return 3;
        }
        if (0x14 < param_2) {
            return 2;
        }
        if (param_2 != 0) {
            return 1;
        }
        break;
    case 1:
        if (0x28 < param_2) {
            return 4;
        }
        if (0x1e < param_2) {
            return 3;
        }
        if (0x14 < param_2) {
            return 2;
        }
        if (param_2 != 0) {
            return 1;
        }
        break;
    case 2:
        if (0x14 < param_2) {
            return 3;
        }
        if (10 < param_2) {
            return 2;
        }
        if (param_2 != 0) {
            return 1;
        }
    }
    return 0;
}
