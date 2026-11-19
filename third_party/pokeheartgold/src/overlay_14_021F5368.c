#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F5368(UnkStruct_ov14_boxapp *arg0, UnkStruct_ov14_mon *arg1) {
    ov14_021F5000(arg0->sys, arg1, 0);
    ov14_021F5054(arg0->sys, arg1, 1);
    ov14_021F50A0(arg0->sys, arg1, 2);
    ov14_021F5114(arg0->sys, arg1, 4);
    ov14_021F5190(arg0, arg1, 5);
    ov14_021F521C(arg0->sys, arg1, 6);
    ov14_021F528C(arg0->sys, arg1, 7);
    ov14_021F52FC(arg0->sys, arg1, 8);
}

void ov14_021F53C0(UnkStruct_ov14_boxsys *param_1) {
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[2]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[4]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[7]);
    ClearWindowTilemapAndScheduleTransfer(&param_1->windows[8]);
    return;
}
