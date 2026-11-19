#include "global.h"

s32 ov70_02243F54(void *, s32);

extern void *ov70_02245E84;

extern void *ov70_02245F58;

extern s32 ov70_02245F5C;

s32 ov70_02243F7C(void *arg0, s32 arg1);

s32 ov70_02243F7C(void *arg0, s32 arg1) {
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r4;
    s32 var_r5;

    temp_r4 = *(s32 *)(&ov70_02245E84 + arg1);
    temp_r1 = arg1 * 2;
    temp_r2 = *(s32 *)(&ov70_02245F5C + temp_r1);
    if (temp_r2 > 0) {
        var_r5 = 0;
        if (temp_r2 > 0) {
            do {
                if (ov70_02243F54(arg0, temp_r4 + var_r5) > 0) {
                    return 1;
                }
                var_r5 += 1;
            } while (var_r5 < (s32) * (s32 *)((u8 *)(&ov70_02245F58 + temp_r1) + 4));
        }
    } else {
        if (ov70_02243F54(arg0, temp_r4) > 0) {
            return 1;
        }
    }
    return 0;
}
