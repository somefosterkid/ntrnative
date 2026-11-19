#include "global.h"

s32 ov73_021E6CC0(void *arg0, s32 arg1);

s32 ov73_021E6CC0(void *arg0, s32 arg1) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 0x324);
    *(s32 *)((u8 *)arg0 + 0x324) = (s32)(v0 + 1);
    if (v0 > 0x3C) {
        *(s32 *)((u8 *)arg0 + 0x318) = 0xF;
    }
    return arg1;
}

s32 ov73_021E66F0(void *);

s32 ov73_021E670C(void *, s32);

s32 ov73_021E756C(void *, s32, s32);

s32 ov73_021E6CDC(void *arg0, s32 arg1);

s32 ov73_021E6CDC(void *arg0, s32 arg1) {
    ov73_021E756C(arg0, 5, 0);
    ov73_021E670C(arg0, 0xA);
    ov73_021E66F0(arg0);
    return arg1;
}
