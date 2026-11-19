#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E65C4(void *);
s32 ov14_021E7148(void *, void *);
s32 ov14_021E9194(UnkStruct_ov14_boxapp *arg0);

s32 sub_02019978(void *, s32);

s32 ov14_021E9194(UnkStruct_ov14_boxapp *arg0) {
    u16 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    sub_02019978(*((void **)&v1->unk_2F0), 0xA);
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        ov14_021E70E0(arg0, arg0->unk21);
        v1->unk_10 = (u16)(v1->unk_10 + 1);
    case 1:
        if (ov14_021E65C4(arg0) == 0) {
            ov14_021E7148(arg0, v1->unk_C);
            v1->unk_10 = 0U;
            return 0;
        }
    default:
        return 1;
    }
}
