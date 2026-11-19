#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

s32 ov96_021FB60C(UnkStruct_ov96_021F94A8 *arg0);

// TRUE when all three lanes reached state 4.
#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021FB60C(UnkStruct_ov96_021F94A8 *arg0) {
    s32 v1;

    for (v1 = 0; v1 < 3; v1++) {
        if (arg0->rows[v1].unk_28 != 4) {
            break;
        }
    }
    return v1 == 3;
}
#else
// DS form: the work pointer steps one row (0x6C) per lane, 0x108 == rows[0].unk_28.
s32 ov96_021FB60C(UnkStruct_ov96_021F94A8 *arg0) {
    s32 v1;
    s32 v2;
    u8 *v0;

    v0 = (u8 *)arg0;
    v2 = 0;
    v1 = 0;
loop_1:
    if (*(s32 *)(v0 + 0x108) == 4) {
        v1 += 1;
        v0 += sizeof(UnkStruct_ov96_021FAB24);
        if (v1 < 3) {
            goto loop_1;
        }
    }
    if (v1 == 3) {
        v2 = 1;
    }
    return v2;
}
#endif
