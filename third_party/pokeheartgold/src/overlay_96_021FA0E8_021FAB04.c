#include "global.h"

s32 ov96_021FAB04(s32 arg0, void *arg1);

s32 ov96_021FAB04(s32 arg0, void *arg1) {
    s32 v0;
    s32 v1;

    v1 = 0;
    v0 = *(s16 *)((u8 *)arg1 + 2) + *(s16 *)((u8 *)arg1 + 6);
    if ((arg0 >= v0) && ((s32)(arg0 - 0x18) < v0)) {
        v1 = 1;
    }
    return v1;
}
