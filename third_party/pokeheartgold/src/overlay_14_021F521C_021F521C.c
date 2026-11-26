#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F4FBC(UnkStruct_ov14_boxsys *, void *, s32, s32, s32, s32, s32, s32, s32);

void ov14_021F521C(UnkStruct_ov14_boxsys *arg0, UnkStruct_ov14_mon *arg1, s32 arg2) {
    s32 sp14;
    s32 var_r7;
    Window *var_r4;
    UnkStruct_ov14_boxsys *var_r5;
    UnkStruct_ov14_mon *var_r6;

    var_r5 = arg0;
    var_r4 = var_r5->windows;
    var_r6 = arg1;
    var_r7 = arg2;
    sp14 = arg2;
    FillWindowPixelBuffer(&var_r4[var_r7], 0U);
    if (var_r6->isEgg == 0) {
        BufferNatureName(var_r5->unk_24, 0U, (u32) var_r6->nature);
        ov14_021F4FBC(var_r5, var_r5->unk_20, sp14, 0x55, 0, 0, 0, 0x10200, 0);
    } else {
        ov14_021F4F84(var_r5, var_r5->unk_20, sp14, 0x5D, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&var_r4[var_r7]);
}

void ov14_021F528C(UnkStruct_ov14_boxsys *arg0, UnkStruct_ov14_mon *arg1, s32 arg2) {
    s32 sp14;
    s32 var_r7;
    Window *var_r4;
    UnkStruct_ov14_boxsys *var_r5;
    UnkStruct_ov14_mon *var_r6;

    var_r5 = arg0;
    var_r4 = var_r5->windows;
    var_r6 = arg1;
    var_r7 = arg2;
    sp14 = arg2;
    FillWindowPixelBuffer(&var_r4[var_r7], 0U);
    if (var_r6->isEgg == 0) {
        BufferAbilityName(var_r5->unk_24, 0U, (u32) var_r6->ability);
        ov14_021F4FBC(var_r5, var_r5->unk_20, sp14, 0x54, 0, 0, 0, 0x10200, 0);
    } else {
        ov14_021F4F84(var_r5, var_r5->unk_20, sp14, 0x5D, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&var_r4[var_r7]);
}
