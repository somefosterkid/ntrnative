// libdwcutil's str.c (Black's ov016 0x021d7200-0x021d7274): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"


s32 DWCi_STRlNLen(s32 s, s32 maxlen) {
    s32 i;

    i = 0;
    if (0 < maxlen) {
        do {
            if (*(u8 *)(s + i) == 0) {
                break;
            }
            i = i + 1;
        } while (i < maxlen);
    }
    return i;
}

s32 func_ov016_021d722c(void *param_1, void *param_2, s32 param_3) {
    s32 val;
    u8 *ptr;
    s32 i;

    i = 0;
    if (0 < param_3 + -1) {
        do {
            ptr = (u8 *)((u8 *)param_2 + i);
            if (*ptr == 0) break;
            val = i * 2;
            i = i + 1;
            *(u16 *)((u8 *)param_1 + val) = (u16)*ptr;
        } while (i < param_3 + -1);
    }
    *(u16 *)((u8 *)param_1 + i * 2) = 0;
    return i;
}
