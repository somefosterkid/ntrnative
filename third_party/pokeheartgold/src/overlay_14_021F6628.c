#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "render_window.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6628(void);
void ov14_021F6638(Window *arg0);
void ov14_021F6654(UnkStruct_ov14_boxsys *param_1, s32 param_2);
void ov14_021F6678(u8 *param_1, s32 param_2);
void ov14_021F6688(u8 *param_1, s32 param_2);

void ov14_021F6628(void) {
    NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x19, HEAP_ID_10);
}

s32 sub_0200E948(Window *, s32, s32);

void ov14_021F6638(Window *window) {
    FillWindowPixelBuffer(window, 0xFU);
    sub_0200E948(window, 0x38E, 0xA);
}

void ov14_021F6654(UnkStruct_ov14_boxsys *param_1, s32 param_2) {
    u8 bgId;

    ClearFrameAndWindow2(&param_1->windows[param_2], 1);
    bgId = GetWindowBgId(&param_1->windows[param_2]);
    ScheduleBgTilemapBufferTransfer(param_1->unk_14, bgId);
    return;
}

void ov14_021F6678(u8 *param_1, s32 param_2) {
    ClearFrameAndWindow2((Window *)(param_1 + 0x30 + param_2 * sizeof(Window)), 1);
    return;
}

void ov14_021F6688(u8 *param_1, s32 param_2) {
    ClearFrameAndWindow2((Window *)(param_1 + 0x30 + param_2 * sizeof(Window)), 0);
    return;
}
