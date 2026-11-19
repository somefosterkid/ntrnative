#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

s32 ov96_021EB06C(void *, s32, s32, s32 *, s32 *);

s32 ov96_0220D8C4(UnkStruct_ov96_0220E3F0_Unit *arg0, s32 arg1, s32 arg2);

s32 ov96_0220D8C4(UnkStruct_ov96_0220E3F0_Unit *arg0, s32 arg1, s32 arg2) {
    s32 v1;
    s32 v0;

    ov96_021EB06C(arg0->unk_4, arg1 >> 0xC, arg2 >> 0xC, &v1, &v0);
    if ((v1 <= 0) || (v0 <= 0) || (v1 >= 0xFF) || (v0 >= 0xFF)) {
        return 1;
    }
    if ((v1 < 8) || (v1 >= 0xF8) || (v0 < 8) || (v0 >= 0xB4)) {
        return 1;
    }
    return 0;
}
