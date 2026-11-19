#include "global.h"

s32 ov70_022429A0(void *arg0, s32 arg1);

s32 ov70_022429A0(void *arg0, s32 arg1) {
    switch (arg1) {
    case 9:
        return -2;
    case 10:
        return 0xA;
    default:
        return arg1;
    }
}
