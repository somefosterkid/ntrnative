#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E76D0(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, u32 tileStart);
s32 ov14_021E7740(void *, s32, s32, s32, s32);

void ov14_021E7808(struct UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov14_021E7808(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ov14_021E76D0(arg0, arg1, arg2, arg3);
    ov14_021E7720(arg0, arg1, arg4);
    ov14_021E7740(arg0, arg1, arg2, arg3, arg4);
    ScheduleBgTilemapBufferTransfer(arg0->sys->unk_14, 3U);
}
