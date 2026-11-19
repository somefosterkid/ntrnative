#include "global.h"

#include "overlay_49.h"

s32 ov49_02265980(s32, s32, s32, void *);

s32 ov49_02267D98(s32, s32 *, s32, s32, s32, s32);

extern s32 ov49_0226A454;

extern void *ov49_0226A70C;

extern u8 ov49_0226A7A8;

void ov49_0226639C(s32 arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    u8 *v2;

    v2 = &ov49_0226A7A8;
    v0 = arg0;
    v1 = 0;
    do {
        ov49_02265980(v0, arg1, v1, &ov49_0226A70C + *v2);
        v1 += 1;
        v2 += 1;
    } while (v1 < 0x10);
    ov49_02267D98(arg1 + 0x954, &ov49_0226A454, 2, 0x21, 0, arg1);
}
