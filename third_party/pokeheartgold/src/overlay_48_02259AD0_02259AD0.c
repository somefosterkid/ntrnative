#include "global.h"

#include "overlay_48.h"

s32 ov48_02259B10(u8 *, s32, s32);

s32 ov48_02259B3C(u8 *, s32, s32);

s32 ov48_02259B68(void *, s32);

s32 ov48_02259BBC();

s32 ov48_02259AD0(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v1;
    s32 v0;
    s32 v2;
    s32 v3;
    s32 v4;

    v0 = arg2;
    v1 = arg3;
    v4 = 3;
    v3 = ov48_02259BBC();
    v2 = ov48_02259B10(arg0, arg1, arg2);
    if (v2 < v3) {
        v4 = ov48_02259B68(arg0, v2);
        ov48_02259B3C(arg0, v1, v2);
    }
    return v4;
}
