#include "global.h"

#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7AD4(UnkStruct_ov14_boxapp *, u16, u32, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EA928(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021E7B8C();

s32 ov14_021EA928(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxapp *var_r5;
    u16 temp_r1;
    u8 temp_r1_2;
    UnkStruct_ov14_boxsys *var_r4;

    var_r5 = arg0;
    var_r4 = var_r5->sys;
    temp_r1 = var_r4->unk_10;
    switch (temp_r1) {
    case 0:
        ov14_021F391C(var_r4, 1);
        ov14_021F29E4(var_r5->sys, 0xB, 1);
        ov14_021F2A18(var_r5->sys, 0xB, 1);
        var_r4->unk_10 = 1U;
        break;
    case 1:
        if (ov14_021F2A04(var_r4, 0xB) == 0) {
            ov14_021F391C(var_r5->sys, 0);
            var_r4->unk_10 = 2U;
        }
        break;
    case 2:
        temp_r1_2 = var_r5->unk21;
        var_r5->unk21 = *(u16 *)var_r4->unk_C;
        ov14_021E7AD4(arg0, temp_r1_2, temp_r1_2, 2);
        var_r4->unk_10 = 3U;
        break;
    case 3:
        if (ov14_021E7B8C() == 0) {
            PlaySE(0x5EAU);
            var_r4->unk_10 = 4U;
        }
        break;
    case 4:
        ov14_021F391C(var_r4, 1);
        ov14_021F29E4(var_r5->sys, 0xB, 2);
        var_r4->unk_10 = 5U;
        break;
    case 5:
        if (ov14_021F2A04(var_r4, 0xB) == 0) {
            ov14_021F2A18(var_r5->sys, 0xB, 0);
            ov14_021F391C(var_r5->sys, 0);
            var_r4->unk_10 = 6U;
        }
        break;
    case 6:
        var_r4->unk_10 = 0U;
        return 0;
    }
    return 1;
}
