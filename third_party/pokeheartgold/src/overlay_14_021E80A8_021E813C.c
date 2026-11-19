#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F32E0(void *);

s32 sub_02019978(void *, s32);

s32 sub_02019B1C(void *, s32, void *, void *);

s32 ov14_021E813C(UnkStruct_ov14_boxapp *arg0) {
    s8 sp0[4];
    s32 temp_r4;

    sub_02019B1C(((void *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 1, &sp0[3], &sp0[2]);
    temp_r4 = sub_02019978(((void *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 1);
    sub_02019B1C(((void *)((UnkStruct_ov14_boxsys *)arg0->sys)->unk_2F0), 1, &sp0[1], &sp0[0]);
    if ((sp0[3] != sp0[1]) || (sp0[2] != sp0[0])) {
        ov14_021F32E0(arg0);
    }
    if (temp_r4 != 0) {
        return 1;
    }
    return 0;
}
