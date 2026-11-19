#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4FBC(UnkStruct_ov14_boxsys *param_1, MsgData *param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, enum Font param_7, u32 param_8, s32 param_9);

void ov14_021F52FC(UnkStruct_ov14_boxsys *param_1, UnkStruct_ov14_mon *param_2, s32 param_3) {
    FillWindowPixelBuffer(&param_1->windows[param_3], 0);
    if (param_2->item != 0) {
        BufferItemName(param_1->unk_24, 0, (u32) param_2->item);
        ov14_021F4FBC(param_1, param_1->unk_20, param_3, 0x56, 0, 0, FONT_SYSTEM, 0x10200, 0);
    } else {
        ov14_021F4F84(param_1, param_1->unk_20, param_3, 0x5c, 0, 0, FONT_SYSTEM, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&param_1->windows[param_3]);
    return;
}
