#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F4FBC(UnkStruct_ov14_boxsys *, MsgData *, s32, s32, s32, s32, s32, s32, s32);
MsgData *ov14_021F6628(void);
s32 ov14_021F6638(void *);
void ov14_021F6698(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2);

void ov14_021F6698(UnkStruct_ov14_boxapp *arg0, s32 arg1, s32 arg2) {
    s32 sp14;
    MsgData *temp_r7;
    s32 temp_r6;

    sp14 = arg1;
    temp_r7 = ov14_021F6628();
    temp_r6 = arg2 * 0x10;
    ov14_021F6638((u8 *)arg0->sys + 0x30 + temp_r6);
    ov14_021F4FBC(arg0->sys, temp_r7, arg2, sp14, 0, 0, 1, 0x1020F, 0);
    ScheduleWindowCopyToVram((struct Window *)((u8 *)arg0->sys + 0x30 + temp_r6));
    DestroyMsgData(temp_r7);
}
