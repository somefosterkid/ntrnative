#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F566C(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F566C(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxsys *v1;
    UnkStruct_ov14_boxsys *v2;

    FillWindowPixelBuffer(&arg0->sys->windows[17], 0U);
    FillWindowPixelBuffer(&arg0->sys->windows[18], 0U);
    FillWindowPixelBuffer(&arg0->sys->windows[19], 0U);
    v0 = arg0->sys;
    ov14_021F4F84(v0, *((void **)&v0->unk_20), 0x11, 0x57, 0, 0, 0, 0x10200, 0);
    v1 = arg0->sys;
    ov14_021F4F84(v1, *((void **)&v1->unk_20), 0x12, 0x58, 0, 0, 0, 0x10200, 0);
    v2 = arg0->sys;
    ov14_021F4F84(v2, *((void **)&v2->unk_20), 0x13, 0x59, 0, 0, 0, 0x10200, 0);
    CopyWindowToVram(&arg0->sys->windows[17]);
    CopyWindowToVram(&arg0->sys->windows[18]);
    CopyWindowToVram(&arg0->sys->windows[19]);
}
