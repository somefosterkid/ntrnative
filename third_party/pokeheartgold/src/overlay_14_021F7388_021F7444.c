#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E5A50(UnkStruct_ov14_boxsys *sys, UnkFuncPtr_ov14_boxtask task);
s32 ov14_021E76B8(void *);
s32 ov14_021F7AC4(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021E9F20(UnkStruct_ov14_boxapp *app);

s32 ov14_021EA180(UnkStruct_ov14_boxapp *app);

void ov14_021F7444(UnkStruct_ov14_boxapp *arg0, u8 arg1, s32 arg2) {
    s32 var_r4;
    var_r4 = arg1;
    if (ov14_021E8544(arg0->sys->unk_2F0) == 1) {
        if ((var_r4 <= 6) && (arg2 >= 7)) {
            ov14_021E76B8(arg0);
            ov14_021F7AC4(*((void * *)&arg0->sys), var_r4, arg2);
            ov14_021E5A50(*((void * *)&arg0->sys), ov14_021EA180);
            return;
        }
    } else if ((var_r4 == 0xC) && (arg2 == 7)) {
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 7);
        var_r4 = 7;
    }
    ov14_021F7AC4(*((void * *)&arg0->sys), var_r4, arg2);
    ov14_021E5A50(*((void * *)&arg0->sys), ov14_021E9F20);
}
