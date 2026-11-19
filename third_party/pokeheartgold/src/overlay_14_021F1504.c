#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F1004();
s32 ov14_021F1504(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021F2270(void *, s32, s32);
void ov14_021F1534(void *arg0);

s32 ov14_021F1504(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F1004();
    GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)(u8)((s32) arg0->unk25 % 6));
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x61;
}

void ov14_021F1534(void *arg0) {
    ov14_021F2270(arg0, 0xA, 0x62);
}
