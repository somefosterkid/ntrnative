#include "global.h"

#include "touchscreen.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7960(s16, s16);
s32 ov14_021F395C(struct UnkStruct_ov14_boxsys *, s16, s16);
s32 ov14_021F3B3C(void *);
s32 ov14_021F3B5C(void *);

s32 ov14_021E6AA0(void *, s32, s32);
s32 ov14_021E7AD4(void *, u16, u32, s32);
s32 ov14_021E8434(void *);
u16 ov14_021E8514(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A04(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021EA4C8(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(void *, s32);

s32 ov14_021E7B8C(void *);
s32 ov14_021EA4C8(UnkStruct_ov14_boxapp *arg0) {
    u32 sp4;
    u32 sp0;
    u16 temp_r0;
    u16 var_r6;
    u16 var_r7;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r0_3;
    UnkStruct_ov14_boxsys *temp_r1;
    UnkStruct_ov14_boxsys *temp_r1_2;
    UnkStruct_ov14_boxapp *var_r4;
    UnkStruct_ov14_boxsys *var_r5;

    var_r4 = arg0;
    var_r5 = var_r4->sys;
    var_r6 = ov14_021E8514(((void *)var_r5->unk_2F0));
    sub_02019978(((void *)var_r4->sys->unk_2F0), 8);
    sub_02019978(((void *)var_r4->sys->unk_2F0), 9);
    sub_02019978(((void *)var_r4->sys->unk_2F0), 0xA);
    temp_r0 = var_r5->unk_10;
    switch ((u32)temp_r0) {
    case 0:
        temp_r0_2 = var_r4->sys;
        if (temp_r0_2->unk_88C8 != 0) {
            ov14_021F391C(temp_r0_2, 1);
            ov14_021F29E4(((void *)var_r4->sys), 0xB, 1);
            ov14_021F2A18(((void *)var_r4->sys), 0xB, 1);
            var_r5->unk_10 = 1U;
        } else {
            ov14_021E8434(((void *)temp_r0_2->unk_2F0));
            var_r5->unk_10 = 0xAU;
        }
        break;
    case 1:
        if (ov14_021F2A04(((void *)var_r4->sys), 0xB) != 1) {
            ov14_021F391C(((void *)var_r4->sys), 0);
            ov14_021F3B3C(((void *)var_r4->sys));
            var_r5->unk_10 = 2U;
        case 2:
            if (System_GetTouchHeldCoords(&sp4, &sp0) == 0) {
                temp_r1 = var_r4->sys;
                var_r6 = ov14_021E7960((s16) *((u32 *)&temp_r1->unk_40B8), (s16) *((u32 *)&temp_r1->unk_40BC));
                var_r7 = var_r6;
                if (var_r6 == 0xFF) {
                    var_r6 = (u16)var_r4->unk21;
                } else if (ov14_021E6AA0(var_r4, var_r4->unk21, var_r6) == 0) {
                    var_r6 = (u16)var_r4->unk21;
                }
                if (var_r6 == var_r4->unk21) {
                    temp_r1_2 = var_r4->sys;
                    ov14_021E8434(((void *)temp_r1_2->unk_2F0));
                } else {
                    temp_r1_2 = var_r4->sys;
                    ov14_021E88F8(((void *)temp_r1_2->unk_2F0));
                }
                ov14_021E7AD4(var_r4, var_r7, var_r6, 2);
                var_r5->unk_10 = 3U;
            } else {
                ov14_021F395C(((void *)var_r4->sys), (s16)sp4, (s16)sp0);
                ov14_021F3B5C(((void *)var_r4->sys));
                *((u32 *)&var_r4->sys->unk_40B8) = sp4;
                *((u32 *)&var_r4->sys->unk_40BC) = sp0;
            }
        }
        break;
    case 3:
        if (ov14_021E7B8C(var_r4) == 0) {
            var_r5->unk_10 = 0xAU;
        }
        break;
    case 10:
        if (var_r6 == 0) {
            temp_r0_3 = var_r4->sys;
            if (temp_r0_3->unk_88C8 != 0) {
                ov14_021F391C(temp_r0_3, 0);
                ov14_021F3B3C(((void *)var_r4->sys));
            }
            var_r5->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
