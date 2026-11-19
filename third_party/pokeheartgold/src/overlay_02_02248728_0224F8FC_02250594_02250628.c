#include "global.h"

u32 ov02_02250628(u32 param_1, u32 param_2);

u32 ov02_02250628(u32 param_1, u32 param_2) {
    switch (param_1) {
    case 1:
        if (param_2 == 0x7f) {
            return 1;
        }
        break;
    case 2:
        if (((s32)param_2 >= 0x64) && ((s32)param_2 < 0x7F)) {
            return 1;
        }
        break;
    case 3:
        if (((s32)param_2 >= 0x32) && ((s32)param_2 < 0x64)) {
            return 1;
        }
        break;
    case 4:
        if (((s32)param_2 >= 0x1E) && ((s32)param_2 < 0x32)) {
            return 1;
        }
        break;
    case 5:
        if (((s32)param_2 > -30) && ((s32)param_2 < 0x1E)) {
            return 1;
        }
        break;
    case 6:
        if (((s32)param_2 > -50) && ((s32)param_2 <= -30)) {
            return 1;
        }
        break;
    case 7:
        if (((s32)param_2 > -127) && ((s32)param_2 <= -50)) {
            return 1;
        }
        break;
    case 8:
        if (param_2 == 0xffffff81) {
            return 1;
        }
        break;
    case 9:
        if ((s32)param_2 >= 0) {
            return 1;
        }
        break;
    case 10:
        if ((s32)param_2 <= -1) {
            return 1;
        }
    }
    return 0;
}
