#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

void ov45_0222F78C(void);

void ov45_0222F78C(void) {
    _022577C0->unk_133 = 0;
    _022577C0->unk_132 = 0;
}

s32 ov45_0222F7A8(s32 arg0);

s32 ov45_0222F7A8(s32 arg0) {
    s32 v0;

    v0 = arg0 - 1;
    if (v0 < 0) {
        v0 = 4;
    }
    return v0;
}

s32 ov45_0222F7B0(s32 arg0);

s32 ov45_0222F7B0(s32 arg0) {
    return arg0 + 1;
}
