#include "global.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E8514(void *arg0);

s32 sub_02019978(void *, s32);

u32 ov14_021E8514(void *arg0) {
    u32 v0;
    u32 v1;

    v1 = 0;
    v0 = 0;
    do {
        if (sub_02019978(arg0, v0 + 3) == 1) {
            v1 = 1;
        }
        v0 += 1;
    } while (v0 < 5U);
    if (sub_02019978(arg0, 0xB) == 1) {
        v1 = 1;
    }
    return v1;
}
