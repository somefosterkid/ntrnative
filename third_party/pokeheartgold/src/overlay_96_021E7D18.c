#include "global.h"
#include "error_handling.h"

s32 ov96_021E7D30(s32, s32, s32);

s32 ov96_021E7D18(s32 arg0, s32 arg1, s32 arg2);

s32 ov96_021E7D30(s32 arg0, s32 arg1, s32 arg2);

s32 ov96_021E7D18(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 == 0xFFFF) {
        return 1;
    }
    return ov96_021E7D30(arg0, arg1, arg2);
}

s32 ov96_021E7D30(s32 arg0, s32 arg1, s32 arg2) {
    s32 v0;

    v0 = 0;
    if (arg2 == 0) {
        if (arg0 == 0xFFFF) {
            v0 = 1;
        } else if (arg0 < arg1) {
            v0 = 1;
        }
    } else if (arg2 == 1) {
        if (arg0 == 0xFFFF) {
            v0 = 1;
        } else if (arg0 > arg1) {
            v0 = 1;
        }
    } else {
        GF_AssertFail();
    }
    return v0;
}
