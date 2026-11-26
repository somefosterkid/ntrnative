#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

extern WindowTemplate ov14_021F84B4[];

void ov14_021F4ED0(UnkStruct_ov14_boxapp *arg0) {
    u32 var_r6;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_10);
    var_r6 = 0;
    do {
        AddWindow(arg0->sys->unk_14, &arg0->sys->windows[var_r6], &ov14_021F84B4[var_r6]);
        var_r6 += 1;
    } while (var_r6 < 0x2CU);
}
