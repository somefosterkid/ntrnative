#include "global.h"

#include "overlay_49.h"

extern u8 ov49_02269634;

s32 ov49_02258A30(s32 arg0) {
    u32 v0;
    u8 *v1;

    v1 = &ov49_02269634;
    v0 = 0;
loop_1:
    if (arg0 == *v1) {
        return 1;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 0x20U) {
        return 0;
    }
    goto loop_1;
}

extern u8 _02269624;

s32 ov49_02258A50(s32 arg0) {
    u32 v0;
    u8 *v1;

    v1 = &_02269624;
    v0 = 0;
loop_1:
    if (arg0 == *v1) {
        return 1;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 3U) {
        return 0;
    }
    goto loop_1;
}

extern u8 ov49_0226962C;

s32 ov49_02258A70(s32 arg0) {
    u32 v0;
    u8 *v1;

    v1 = &ov49_0226962C;
    v0 = 0;
loop_1:
    if (arg0 == *v1) {
        return 1;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 7U) {
        return 0;
    }
    goto loop_1;
}

extern u8 ov49_02269628;

s32 ov49_02258A90(s32 arg0) {
    u32 v0;
    u8 *v1;

    v1 = &ov49_02269628;
    v0 = 0;
loop_1:
    if (arg0 == *v1) {
        return 1;
    }
    v0 += 1;
    v1 += 1;
    if (v0 >= 4U) {
        return 0;
    }
    goto loop_1;
}
