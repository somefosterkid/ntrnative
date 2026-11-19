#include "global.h"

#include "bg_window.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_box_funcs.h"

void ov14_021F69F0(struct UnkStruct_ov14_boxapp *param_1, s32 param_2, u32 param_3, u32 param_4);

void sub_0200E948(void *, u32, u32, u32);

void ov14_021F69F0(UnkStruct_ov14_boxapp *param_1, s32 param_2, u32 param_3, u32 param_4) {
    sub_0200E948(*((u8 **)&param_1->sys) + 0x30 + param_2 * 0x10, 0x38e, 10, param_4);
    ScheduleWindowCopyToVram((Window *)(*((u8 **)&param_1->sys) + 0x30 + param_2 * (s32)sizeof(Window)));
    return;
}
