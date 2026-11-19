#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);
void ov14_021F4848(u8 *);
void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F48B4(void *);
s32 ov14_021F49E0(void *);
s32 ov14_021F57B8(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F6D14(void *arg0, s32 arg1, s32 arg2) {
    s32 var_r4;
    u8 *new_var;
    u8 *new_var2;
    int var_r1;
    unsigned int var_r1_2;
    void *temp_r1;
    var_r4 = arg1;
    new_var = (u8 *)arg0;
    if ((var_r4 >= 0) && (var_r4 <= 5)) {
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0xE);
    } else {
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 8);
    }
    if ((arg2 == 8) && (var_r4 == 0)) {
        temp_r1 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
        var_r4 = ((UnkStruct_ov14_boxsys *)temp_r1)->unk_43C;
        GridInputHandler_SetNextLastUnk0FInputs(((UnkStruct_ov14_boxsys *)temp_r1)->unk_2C, (s32)((u8)var_r4), 8, 8);
    }
    if ((var_r4 >= 0) && (var_r4 <= 5)) {
        ((UnkStruct_ov14_boxapp *)new_var)->sys->unk_43C = var_r4;
    }
    if ((var_r4 == 0) && (arg2 == 5)) {
        if (((s32)((((UnkStruct_ov14_boxapp *)arg0)->unk25) + 1)) >= 0x12) {
            var_r1 = 0;
        } else {
            var_r1 = (((UnkStruct_ov14_boxapp *)new_var)->unk25) + 1;
        }
        ((UnkStruct_ov14_boxapp *)arg0)->unk25 = var_r1;
        ov14_021F49E0(arg0);
        ov14_021F48B4(arg0);
        ov14_021F4848((u8 *)arg0);
        ov14_021F57B8(arg0);
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)new_var)->sys), 5, 4);
        ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), var_r4, arg2);
        ov14_021E5A50(((void *)((UnkStruct_ov14_boxapp *)new_var)->sys), ov14_021E9F20);
        return;
    }
    if ((var_r4 == 5) && (arg2 == 0)) {
        if (((s32)((((UnkStruct_ov14_boxapp *)arg0)->unk25) - 1)) < 0) {
            var_r1_2 = 0x11;
        } else {
            var_r1_2 = (((UnkStruct_ov14_boxapp *)new_var)->unk25) - 1;
        }
        ((UnkStruct_ov14_boxapp *)arg0)->unk25 = var_r1_2;
        ov14_021F49E0(arg0);
        ov14_021F48B4(arg0);
        ov14_021F4848((u8 *)arg0);
        ov14_021F57B8(arg0);
        ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 4, 2);
        new_var2 = new_var + 0x34;
        ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), var_r4, arg2);
        ov14_021E5A50(*((void **)new_var2), ov14_021E9F20);
        return;
    }
    if (((0 <= var_r4) && (var_r4 <= 5)) && (arg2 != 8)) {
        ((UnkStruct_ov14_boxapp *)new_var)->unk25 = (u8)(var_r4 + (6 * (((s32)(((UnkStruct_ov14_boxapp *)new_var)->unk25)) / 6)));
        ov14_021F48B4(arg0);
        ov14_021F57B8(arg0);
    }
    ov14_021F7AC4(((void *)((UnkStruct_ov14_boxapp *)new_var)->sys), var_r4, arg2);
    ov14_021E5A50(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), ov14_021E9F20);
}
