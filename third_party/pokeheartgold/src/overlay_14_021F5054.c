#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F4FBC(UnkStruct_ov14_boxsys *param_1, MsgData *param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, enum Font param_7, u32 param_8, s32 param_9);

void ov14_021F5054(UnkStruct_ov14_boxsys *param_1, UnkStruct_ov14_mon *param_2, s32 param_3) {
    FillWindowPixelBuffer(&param_1->windows[param_3], 0);
    BufferBoxMonNickname(param_1->unk_24, 0, param_2->boxmon);
    ov14_021F4FBC(param_1, param_1->unk_20, param_3, 1, 0, 0, FONT_SYSTEM, 0x10200, 0);
    ScheduleWindowCopyToVram(&param_1->windows[param_3]);
    return;
}
