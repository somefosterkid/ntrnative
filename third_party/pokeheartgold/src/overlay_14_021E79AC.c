#include "global.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E79AC(s32 arg0, s32 arg1, void *arg2);

s32 ov14_021E79AC(s32 arg0, s32 arg1, void *arg2) {
    void *v1;
    u32 v0;

    v1 = arg2;
    v0 = 0;
loop_1:
    if (ov14_021E7940(arg0, arg1, v1) == 1) {
        return v0 + 0x1E;
    }
    v0 += 1;
    v1 += 4;
    if (v0 >= 6U) {
        return 0xFF;
    }
    goto loop_1;
}
