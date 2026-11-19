#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "string_util.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F4FBC(UnkStruct_ov14_boxsys *, void *, s32, s32, s32, s32, s32, s32, s32);

void ov14_021F50A0(UnkStruct_ov14_boxsys *arg0, UnkStruct_ov14_mon *arg1, s32 arg2) {
    s32 sp14;
    s32 var_r6;
    void *var_r4;
    void *var_r5;
    UnkStruct_ov14_mon *var_r7;

    var_r5 = arg0;
    var_r4 = var_r5;
    var_r7 = arg1;
    var_r4 += 0x30;
    var_r6 = arg2 * 0x10;
    sp14 = arg2;
    FillWindowPixelBuffer(var_r4 + var_r6, 0U);
    if (var_r7->isEgg == 0) {
        sub_0200CDAC(((UnkStruct_ov14_boxsys *)var_r5)->unk_1C, 1U, var_r4 + var_r6, 0U, 5U);
        BufferIntegerAsString(((UnkStruct_ov14_boxsys *)var_r5)->unk_24, 0U, (s32)var_r7->level, 3U, PRINTING_MODE_LEFT_ALIGN, 1);
        ov14_021F4FBC(var_r5, *((void **)&((UnkStruct_ov14_boxsys *)var_r5)->unk_20), sp14, 0x5A, 0x10, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(var_r4 + var_r6);
}
