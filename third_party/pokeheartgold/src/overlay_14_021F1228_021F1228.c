#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F4530(void *);
#else
s32 ov14_021F4530(void *);
#endif
s32 ov14_021F462C(void *);
s32 ov14_021F58B8(void *);
void ov14_021F1228(struct UnkStruct_ov14_boxapp *arg0, s32 arg1);

void ov14_021F1228(UnkStruct_ov14_boxapp *arg0, s32 arg1) {
    s32 var_r3;
    u8 temp_r1;
    UnkStruct_ov14_boxsys *temp_r0;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r2;

    temp_r0 = arg0->sys;
    var_r3 = temp_r0->unk_44D + (arg1 * 4);
    if (var_r3 < 0) {
        var_r3 += 0x18;
    } else if (var_r3 >= 0x18) {
        var_r3 -= 0x18;
    }
    temp_r0->unk_44D = (u8)var_r3;
    temp_r0_2 = arg0->sys;
    if (arg1 > 0) {
        ov14_021F29E4(temp_r0_2, 5, 4);
    } else {
        ov14_021F29E4(temp_r0_2, 4, 2);
    }
    ov14_021F462C(arg0);
    ov14_021F4530(arg0);
    ov14_021F58B8(arg0);
    temp_r2 = arg0->sys;
    temp_r1 = temp_r2->unk_44D;
    GridInputHandler_SetNextInput(temp_r2->unk_2C, (s32)(u8)(temp_r1 % 4));
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
}
