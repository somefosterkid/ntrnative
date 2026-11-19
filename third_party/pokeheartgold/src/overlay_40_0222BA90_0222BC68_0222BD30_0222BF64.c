#include "global.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222BF8C(void *, s32);

void ov40_0222BF64(void *arg0, s32 arg1, s32 arg2, s32 *arg3);

void ov40_0222BF64(void *arg0, s32 arg1, s32 arg2, s32 *arg3) {
    if (arg2 != 0) {
        *arg3 = arg1;
        ov40_0222BF80(arg0, 0);
        ov40_0222BF8C(arg0, 0);
    }
}
