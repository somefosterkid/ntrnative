#include "global.h"

s32 ov07_022220B8(void *, void *, s32, s32);

s32 ov07_022227D8();

s32 ov07_0222283C(void *arg0, s32 arg1, s32 arg2, void *arg3);

s32 ov07_0222283C(void *arg0, s32 arg1, s32 arg2, void *arg3) {
    if (ov07_022227D8() != 0) {
        ov07_022220B8(arg0, arg3, arg1, arg2);
        return 1;
    }
    return 0;
}
