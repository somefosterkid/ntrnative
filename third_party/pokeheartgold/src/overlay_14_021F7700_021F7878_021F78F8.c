#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);
void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F4530(void *);
s32 ov14_021F459C(void *);
s32 ov14_021F462C(void *);
s32 ov14_021F58B8(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

void ov14_021F78F8(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    s32 var_r2;
    s32 var_r2_2;
    s32 var_r4;
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r1;
    var_r4 = arg1;
    if ((var_r4 >= 0) && (var_r4 <= 3)) {
        ov14_021F29E4(*((void * *)&arg0->sys), 9, 0xE);
    } else {
        ov14_021F29E4(*((void * *)&arg0->sys), 9, 8);
    }
    if ((arg2 == 6) && (var_r4 == 0)) {
        temp_r1 = *((void * *)&arg0->sys);
        var_r4 = ((UnkStruct_ov14_boxsys *)temp_r1)->unk_43C;
        GridInputHandler_SetNextLastUnk0FInputs(((UnkStruct_ov14_boxsys *)temp_r1)->unk_2C, (s32)((u8)var_r4), 6, 6);
    }
    if ((var_r4 >= 0) && (var_r4 <= 3)) {
        arg0->sys->unk_43C = var_r4;
    }
    if ((var_r4 == 0) && (arg2 == 3)) {
        temp_r0 = *((void * *)&arg0->sys);
        var_r2 = *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_44D);
        var_r2 += 1;
        if (var_r2 >= 0x18) {
            *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_44D) = 0;
        } else {
            *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0)->unk_44D) = (u8)var_r2;
        }
        ov14_021F462C(arg0);
        ov14_021F4530(arg0);
        ov14_021F459C(arg0);
        ov14_021F58B8(arg0);
        ov14_021F29E4(*((void * *)&arg0->sys), 5, 4);
        ov14_021F7AC4(*((void * *)&arg0->sys), var_r4, arg2);
        ov14_021E5A50(*((void * *)&arg0->sys), ov14_021E9F20);
        return;
    }
    if ((var_r4 == 3) && (arg2 == 0)) {
        temp_r0_2 = *((void * *)&arg0->sys);
        var_r2_2 = *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44D);
        var_r2_2 -= 1;
        if (var_r2_2 < 0) {
            *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44D) = 0x17;
        } else {
            *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44D) = (u8)var_r2_2;
        }
        ov14_021F462C(arg0);
        ov14_021F4530(arg0);
        ov14_021F459C(arg0);
        ov14_021F58B8(arg0);
        ov14_021F29E4(*((void * *)&arg0->sys), 4, 2);
        ov14_021F7AC4(*((void * *)&arg0->sys), var_r4, arg2);
        ov14_021E5A50(*((void * *)&arg0->sys), ov14_021E9F20);
        return;
    }
    if (((var_r4 >= 0) && (var_r4 <= 3)) && (arg2 != 6)) {
        temp_r0_3 = *((void * *)&arg0->sys);
        var_r2_2 = *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_3)->unk_44D);
        var_r2_2 = (u32)var_r2_2 >> 2;
        var_r2_2 = var_r2_2 << 2;
        *((u8 *)&((UnkStruct_ov14_boxsys *)temp_r0_3)->unk_44D) = (u8)(var_r4 + var_r2_2);
        ov14_021F459C(arg0);
        ov14_021F58B8(arg0);
    }
    ov14_021F7AC4(*((void * *)&arg0->sys), var_r4, arg2);
    ov14_021E5A50(*((void * *)&arg0->sys), ov14_021E9F20);
}
