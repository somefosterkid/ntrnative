#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

MsgData *ov14_021F6628(void);
struct UnkStruct_ov14_boxsys;

void ov14_021F5620(void *arg0) {
    MsgData *temp_r4;

    temp_r4 = ov14_021F6628();
    FillWindowPixelBuffer(&((UnkStruct_ov14_boxapp *)arg0)->sys->windows[3], 0U);
    ov14_021F4F84(((UnkStruct_ov14_boxapp *)arg0)->sys, temp_r4, 3, *(s32 *)((u8 *)(*(void **)arg0) + 8) + 0x32, 0, 8, 0, 0x10200, 0);
    DestroyMsgData(temp_r4);
    CopyWindowToVram(&((UnkStruct_ov14_boxapp *)arg0)->sys->windows[3]);
}
