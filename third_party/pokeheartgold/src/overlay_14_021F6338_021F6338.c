#include "global.h"

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6338(UnkStruct_ov14_boxsys *arg0, s32 arg1, s32 arg2);

void ov14_021F6338(UnkStruct_ov14_boxsys *arg0, s32 arg1, s32 arg2) {
    s32 sp14;
    s32 var_r4;
    s32 var_r6;
    u32 temp_r1;
    Window *var_r5;
    UnkStruct_ov14_boxsys *var_r7;

    var_r7 = arg0;
    var_r6 = arg1;
    var_r5 = var_r7->windows;
    var_r4 = var_r6;
    sp14 = arg2;
    FillWindowPixelBuffer(&var_r5[var_r4], 0xDU);
    temp_r1 = GetWindowWidth(&var_r5[var_r4]) * 8;
    ov14_021F4F84(var_r7, var_r7->unk_20, var_r6, sp14, (u32)((temp_r1 + (temp_r1 >> 0x1F)) << 0x17) >> 0x18, 0, 4, 0x90A0D, 2);
    CopyWindowPixelsToVram_TextMode(&var_r5[var_r4]);
}
