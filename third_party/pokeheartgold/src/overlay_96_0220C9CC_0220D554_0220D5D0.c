#include "global.h"

#include "overlay_96_unk_struct_ov96_0220e3f0.h"

s32 ov96_021EAF78(void *, s32, s32, VecFx32 *, s32 *, s32 *);

s32 ov96_0220D5D0(UnkStruct_ov96_0220E3F0_Unit *arg0, UnkStruct_ov96_0220E3F0_Unit *arg1);

s32 ov96_0220D5D0(UnkStruct_ov96_0220E3F0_Unit *arg0, UnkStruct_ov96_0220E3F0_Unit *arg1) {
    VecFx32 v0;
    VecFx32 a;
    VecFx32 b;
    s32 v2;
    unsigned int new_var3;
    s32 v1;
    ov96_021EAF78(arg0->unk_4, arg0->unk_1C.x, arg0->unk_1C.y, &a, &a.y, &v2);
    ov96_021EAF78(arg1->unk_4, arg1->unk_1C.x, arg1->unk_1C.y, &b, &b.y, &v1);
    new_var3 = (v2 + v1) << 0xC;
    a.z = 0;
    b.z = 0;
    VEC_Subtract(&a, &b, &v0);
    if (VEC_Mag(&v0) <= ((s32)new_var3)) {
        return 1;
    }
    return 0;
}
