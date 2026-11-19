#include "global.h"

s32 ov45_0222DE58(void *, s32, s32, s32);

void ov45_0222D8C8(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov45_0222DE74(void *, s32);
s32 ov45_0222DF14(u8 *);
s32 ov45_0222D8D4(u8 *arg0, s32 arg1);

void ov45_0222D8C8(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    ov45_0222DE58(arg0 + 8, arg1, arg2, arg3);
}

s32 ov45_0222D8D4(u8 *arg0, s32 arg1) {
    s32 v0;

    v0 = ov45_0222DF14(arg0 + 8);
    ov45_0222DE74(arg0 + 8, arg1);
    return v0;
}
