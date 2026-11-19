#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

extern WindowTemplate ov14_021F84B4;

void ov14_021F4ED0(UnkStruct_ov14_boxapp *arg0) {
    WindowTemplate *var_r4;
    s32 var_r5;
    u32 var_r6;
    void *temp_r1;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_10);
    var_r6 = 0;
    var_r4 = &ov14_021F84B4;
    var_r5 = 0;
    do {
        temp_r1 = ((void *)arg0->sys);
        AddWindow(((UnkStruct_ov14_boxsys *)temp_r1)->unk_14, temp_r1 + 0x30 + var_r5, var_r4);
        var_r6 += 1;
        var_r4 = (void *)((u8 *)var_r4 + (8));
        var_r5 += 0x10;
    } while (var_r6 < 0x2CU);
}
