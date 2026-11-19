#include "global.h"

unsigned int ov01_021EE934(s32 arg0, s32 arg1, s8 *arg2);

unsigned int ov01_021EE934(s32 arg0, s32 arg1, s8 *arg2) {
    switch (arg0) {
    case 0x1B9:
        if (arg1 == 0) {
            *arg2 = 0x20;
            return 0x7A;
        }
        break;
    case 0x1BB:
    case 0x1BC:
        if (arg1 == 3) {
            *arg2 = 0x10;
            return 0x7D;
        }
        break;
    }

    *arg2 = 0x20;
    return 0x11;
}
