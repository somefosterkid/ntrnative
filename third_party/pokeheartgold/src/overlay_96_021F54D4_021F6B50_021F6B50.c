#include "global.h"

void ov96_021F6B50(s32 arg0, s32 arg1, s32 *arg2);

void ov96_021F6B50(s32 arg0, s32 arg1, s32 *arg2) {
    s32 i;
    s32 neg = -arg1;
    s32 rest = 0x7F - arg0;

    for (i = 0; i < arg0; i++) {
        arg2[i] = -((neg * (u8)(i + 1)) / (arg0 + 1));
    }
    for (i = 0; i < rest; i++) {
        arg2[(u8)(arg0 + 1 + i)] = -((neg * (u8)(rest - i)) / (rest + 1));
    }
}
